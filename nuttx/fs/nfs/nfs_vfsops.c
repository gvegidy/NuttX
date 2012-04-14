/****************************************************************************
 * fs/nfs/nfs_vfsops.c
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
 *   Copyright (C) 2012 Jose Pablo Rojas Vargas. All rights reserved.
 *   Author: Jose Pablo Rojas Vargas <jrojas@nx-engineering.com>
 *
 * Leveraged from OpenBSD:
 *
 *   Copyright (c) 1989, 1993, 1995
 *   The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Rick Macklem at The University of Guelph.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/systm.h>
#include <sys/sysctl.h>
#include <sys/statfs>

#include <queue.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <nuttx/fs/dirent.h>
#include <nuttx/fs/fs.h>

#include <net/if.h>
#include <netinet/in.h>

#include "rpcv2.h"
#include "nfsproto.h"
#include "nfs_node.h"
#include "nfs.h"
#include "nfs_mount.h"
#include "xdr_subs.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define NFS_DIRHDSIZ         (sizeof (struct nfs_dirent) - (MAXNAMLEN + 1))
#define NFS_DIRENT_OVERHEAD  offsetof(struct nfs_dirent, dirent)

/****************************************************************************
 * Private Type Definitions
 ****************************************************************************/

struct nfs_dirent
{
  uint32_t cookie[2];
  struct dirent dirent;
};

/****************************************************************************
 *  External Public Data  (this belong in a header file)
 ****************************************************************************/

extern uint32_t nfs_true, nfs_false;
extern uint32_t nfs_xdrneg1;
extern nfstype nfsv3_type[8];
extern struct nfsstats nfsstats;
extern int nfs_ticks;
extern uint32_t nfs_procids[NFS_NPROCS];

/****************************************************************************
 * Public Data
 ****************************************************************************/

int nfs_numasync = 0;

/* nfs vfs operations. */

const struct mountpt_operations nfs_ops = {
  nfs_open,                     /* open */
  nfs_close,                    /* close */
  nfs_read,                     /* read */
  nfs_write,                    /* write */
  NULL,                         /* seek */
  NULL,                         /* ioctl */
  nfs_sync,                     /* sync */

  NULL,                         /* opendir */
  NULL,                         /* closedir */
  nfs_readdir,                  /* readdir */
  NULL,                         /* rewinddir */

  nfs_mount,                    /* bind */
  nfs_unmount,                  /* unbind */
  nfs_statfs,                   /* statfs */

  nfs_remove,                   /* unlink */
  nfs_mkdir,                    /* mkdir */
  nfs_rmdir,                    /* rmdir */
  nfs_rename,                   /* rename */
  NULL                          /* stat */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/* nfs create struct file
 * if oflags == O_CREAT it creates a file, if not it
 * check to see if the type is ok
 * and that deletion is not in progress.
 */

int
nfs_open(FAR struct file *filp, FAR const char *relpath, // done but check variable how
         int oflags, mode_t mode)
{
  struct inode *in;
  struct nfs_fattr *vap;
  struct nfsv3_sattr *sp;
  struct nfsmount *nmp;
  struct nfsnode *np;
  struct CREATE3args *create;
  struct CREATE3resok *resok;
  void *replydata;
  int error = 0;

  /* Sanity checks */

  DEBUGASSERT(filep->f_inode != NULL);

  /* Get the mountpoint inode reference from the file structure and the
   * mountpoint private data from the inode structure
   */

  in = filep->f_inode;
  nmp = (struct nfsmount*)in->i_private;
  np = (struct nfsnode*)filep->f_priv;

  DEBUGASSERT(nmp != NULL);

  /* Check if the mount is still healthy */

  nfs_semtake(nmp);
  error = nfs_checkmount(nmp);
  if (error != 0)
    {
      goto errout_with_semaphore;
    }

  if (oflags == O_CREAT)
    {
      /* Sanity checks */

      DEBUGASSERT(filep->f_priv == NULL);
again:
      nfsstats.rpccnt[NFSPROC_CREATE]++;
      vap = nmp->nm_head->n_fattr;
      sp->sa_modetrue = nfs_true;
      sp->sa_mode = txdr_unsigned(vap->fa_mode);
      sp->sa_uid = nfs_xdrneg1;
      sp->sa_gid = nfs_xdrneg1;
      sp->sa_size = nfs_xdrneg1;
      sp->sa_atimetype = txdr_unsigned(NFSV3SATTRTIME_TOCLIENT);
      sp->sa_mtimetype = txdr_unsigned(NFSV3SATTRTIME_TOCLIENT);

      txdr_nfsv3time(&vap->fa_atime, &sp->sa_atime);
      txdr_nfsv3time(&vap->fa_mtime, &sp->sa_mtime);

      create->how = sp;
      create->where->dir= nmp->nm_fh;
      create->where->name = relpath;

      error = nfs_request(in, NFSPROC_CREATE, (void *) create, replydata);
      if (!error)
        {
          /* Create an instance of the file private data to describe the opened
           * file.
           */

          np = (struct nfsnode *)zalloc(sizeof(struct nfsnode));
          if (!np)
            {
              fdbg("Failed to allocate private data\n", error);
              error = -ENOMEM;
              goto errout_with_semaphore;
            }

          /* Initialize the file private data (only need to initialize
           * non-zero elements)
           */

          resok = (struct CREATE3resok *) replydata;
          np->n_open        = true;
          np->nfsv3_type    = NFREG;
          np->n_size        = (struct uint64_t *)resok->attributes->nfsv3fa_size;
          np->n_fattr       = resok->attributes;
          np->n_mtime       = (struct timespec*) resok->attributes->nfsv3fa_mtime
          np->n_ctime       = (struct time_t*) resok->attributes->nfsv3fa_ctime

          /* Attach the private date to the struct file instance */

          filep->f_priv = np;

          /* Then insert the new instance into the mountpoint structure.
           * It needs to be there (1) to handle error conditions that effect
           * all files, and (2) to inform the umount logic that we are busy
           * (but a simple reference count could have done that).
           */

          np->n_next = nmp->nm_head;
          nmp->nm_head = np->n_next;
          error = 0;
        }
      else
        {
          if (info_v3 && error == NFSERR_NOTSUPP)
            {
              goto again;
            }
        }

      np->n_flag |= NMODIFIED;
    }
  else
    {
      if (np->nfsv3_type != NFREG && np->nfsv3_type != NFDIR)
        {
          fdbg("open eacces typ=%d\n", np->nfsv3_type);
          return EACCES;
        }

      if (np->n_flag & NMODIFIED)
        {
          if (np->nfsv3_type == NFDIR)
            {
              np->n_direofoffset = 0;
            }
        }
    }

  /* For open/close consistency. */

  NFS_INVALIDATE_ATTRCACHE(np);

errout_with_semaphore:
  nfs_semgive(nmp);
  return error;
}

/****************************************************************************
 * Name: nfs_close
 ****************************************************************************/

int nfs_close(FAR struct file *filep) //done
{
  struct nfsmount *nmp;
  struct nfsnode *np;
  int error = 0;

  fvdbg("Closing\n");

  /* Sanity checks */

  DEBUGASSERT(filep->f_priv != NULL && filep->f_inode != NULL);

  /* Recover our private data from the struct file instance */

  np = filep->f_priv;
  nmp = filep->f_inode->i_private;

  DEBUGASSERT(nmp != NULL);

  if (np->nfsv3_type == NFREG)
    {
      error = nfs_sync(filep);
      kfree(np);
      filep->f_priv = NULL;
    }

  return error;
}

/****************************************************************************
 * Name: nfs_read
 ****************************************************************************/

static ssize_t nfs_read(FAR struct file *filep, char *buffer, size_t buflen) //done
{
  struct nfsmount *nmp;
  struct nfsnode *np;
  unsigned int readsize;
  int bytesleft;
  uint64_t offset;
  void *datareply;
  struct READ3args *read;
  struct READ3resok *resok;
  uint8_t *userbuffer = (uint8_t*)buffer;
  int info_v3;
  int error = 0;
  int len;
  bool eof;

  fvdbg("Read %d bytes from offset %d\n", buflen, filep->f_pos);

  /* Sanity checks */

  DEBUGASSERT(filep->f_priv != NULL && filep->f_inode != NULL);

  /* Recover our private data from the struct file instance */

  np = (struct nfsnode*) filep->f_priv;
  nmp = (struct nfsmount*) filep->f_inode->i_private;
  eof = false;
  offset = 0;

  DEBUGASSERT(nmp != NULL);

  /* Make sure that the mount is still healthy */

  nfafs_semtake(nmp);
  error = nfs_checkmount(nmp);
  if (error != 0)
    {
      fdbg("nfs_checkmount failed: %d\n", error);
      goto errout_with_semaphore;
    }

  if (np->nfsv3_type != NFREG)
    {
      fdbg("read eacces typ=%d\n", np->nfsv3_type);
      return EACCES;
    }

  if ((nmp->nm_flag & (NFSMNT_NFSV3 | NFSMNT_GOTFSINFO)) == NFSMNT_NFSV3)
    {
      (void)nfs_fsinfo(nmp);
    }

  /* Get the number of bytes left in the file */

  bytesleft = np->n_size - filep->f_pos;
  readsize = 0;

  /* Truncate read count so that it does not exceed the number
   * of bytes left in the file.
   */

  if (buflen > bytesleft)
    {
      buflen = bytesleft;
    }

    len = nmp->nm_rsize;
    if (len < buflen)
      {
        error = EFBIG
        goto errout_with_semaphore;

      }

      nfsstats.rpccnt[NFSPROC_READ]++;
again:
      read->file = np->nfsv3_type;
      read->count = buflen;
      read->offset = offset;

      error = nfs_request(nmp, NFSPROC_READ, read, datareply);
      if (error)
        {
          goto errout_with_semaphore;
        }

      resok = (struct READ3resok *) datareply;
      eof = resok->eof;
      if (eof == true)
        {
          readsize = resok->count;
          np->n_fattr = resok->file_attributes;
          memcpy(userbuffer, resok->data, (struct size_t)readsize);

        }
      else
        {
          goto again;
        }

nfs_semgive(nmp);
  return readsize;

errout_with_semaphore:
  nfs_semgive(nmp);
  return error;
}

/****************************************************************************
 * Name: nfs_write
 ****************************************************************************/

static ssize_t
nfs_write(FAR struct file *filep, const char *buffer, size_t buflen) // done
{
  /*struct nfsm_info info;
  uint32_t *tl;
  int32_t t1, backup;
  caddr_t cp2;*/
  struct inode *inode;
  struct nfsmount *nmp;
  struct nfsnode *np;
  unsigned int  writesize;
  void *datareply;
  struct WRITE3args *write;
  struct WRITE3resok resok;
  uint8_t  *userbuffer = (uint8_t*)buffer;
  int error = 0;
  uint64_t  offset;
  int len;
  struct stable_how commit;
  int committed = NFSV3WRITE_FILESYNC;

  /* Sanity checks */

  DEBUGASSERT(filep->f_priv != NULL && filep->f_inode != NULL);

  /* Recover our private data from the struct file instance */

  np     = (struct nfsnode *)filep->f_priv;
  inode  = filep->f_inode;
  nmp    = (struct nfsmount *)inode->i_private;
  offset = 0;

  DEBUGASSERT(nmp != NULL);

  /* Make sure that the mount is still healthy */

  nfs_semtake();
  error = nfs_checkmount(nmp);
  if (error != 0)
    {
      goto errout_with_semaphore;
    }

  /* Check if the file size would exceed the range of off_t */

  if (np->n_size + buflen < np->n_size)
    {
      ret = -EFBIG;
      goto errout_with_semaphore;
    }

  len = nmp->nm_wsize;
  if (len < buflen)
    {
      error = -EFBIG
      goto errout_with_semaphore;
    }
  writesize = 0;

  nfsstats.rpccnt[NFSPROC_WRITE]++;
  write->file = np->nfsv3_type;
  write->offset = offset;
  write->count = buflen;
  write->stable = committed;
  memcpy(write->data, userbuffer, buflen);

  error = nfs_request(nmp, NFSPROC_WRITE, write, datareply);
  if (error)
    {
      goto errout_with_semaphore;
    }

  resok = (struct WRITE3resok *)datareply;
  writesize = resok->count;
  if (writesize == 0)
    {
       error = -NFSERR_IO;
       goto errout_with_semaphore;
    }

  commit = resok->committed;
  np->n_fattr = resok->file_wcc->after;

  /* Return the lowest committment level obtained by any of the RPCs. */

  if (committed == NFSV3WRITE_FILESYNC)
    {
      committed = commit;
    }
  else if (committed == NFSV3WRITE_DATASYNC &&
           commit == NFSV3WRITE_UNSTABLE)
    {
       committed = commit;
    }

  if ((nmp->nm_flag & NFSMNT_HASWRITEVERF) == 0)
    {
      bcopy((void) resok->verf, (void) nmp->nm_verf, NFSX_V3WRITEVERF);
      nmp->nm_flag |= NFSMNT_HASWRITEVERF;
    }
  else if (strncmp((char) resok->verf, (char) nmp->nm_verf, NFSX_V3WRITEVERF))
    {
      bcopy((void) resok->verf, (void) nmp->nm_verf, NFSX_V3WRITEVERF);
    }

  np->n_mtime = np->n_fattr.nfsv3fa_mtime;

  nfs_semgive(nmp);
  return writesize;

errout_with_semaphore:
  nfs_semgive(nmp);
  return error;
}

/* nfs file remove call
 * To try and make nfs semantics closer to ufs semantics, a file that has
 * other processes using the vnode is renamed instead of removed and then
 * removed later on the last close.
 * - If v_usecount > 1
 *    If a rename is not already in the works
 *       call nfs_sillyrename() to set it up
 *     else
 *    do the remove rpc
 */
/****************************************************************************
 * Name: nfs_remove
 *
 * Description: Remove a file
 *
 ****************************************************************************/

int nfs_remove(struct inode *mountpt, const char *relpath) //done
{
  struct nsfmount *nmp;
  struct nfsnode *np;
  void *datreply;
  struct REMOVE3args *remove;
  struct REMOVE3resok resok;
  int error = 0;

  /* Sanity checks */

  DEBUGASSERT(mountpt && mountpt->i_private);

  /* Get the mountpoint private data from the inode structure */

  nmp = (struct nfsmount *)mountpt->i_private;
  np = nmp->nm_head;

  /* Check if the mount is still healthy */

  nfs_semtake(nmp);
  error = fat_checkmount(nmp);
  if (error == 0)
    {
      /* If the file is open, the correct behavior is to remove the file
       * name, but to keep the file cluster chain in place until the last
       * open reference to the file is closed.
       */

      /* Remove the file */

      if (np->n_type != NFREG)
        {
          error = EPERM;
          goto errout_with_semaphore;
        }

      /* Do the rpc */

      nfsstats.rpccnt[NFSPROC_REMOVE]++;
      remove->dir = nmp->nm_fh;
      remove->name = relpath;

      error = nfs_request(nmp, NFSPROC_REMOVE, remove, datareply);

      /* Kludge City: If the first reply to the remove rpc is lost..
       *   the reply to the retransmitted request will be ENOENT
       *   since the file was in fact removed
       *   Therefore, we cheat and return success.
       */

      if (error == ENOENT)
        {
          error = 0;
        }

      if (error)
        {
          goto errout_with_semaphore;
        }

      resok = (struct REMOVE3resok *)datareply;
      np->n_fattr = resok->dir_wcc->after;
      np->n_flag |= NMODIFIED;
    }
   NFS_INVALIDATE_ATTRCACHE(np);

errout_with_semaphore:
   nfs_semgive(nmp);
   return error;
}

/****************************************************************************
 * Name: nfs_rename
 *
 * Description: Rename a file or directory
 *
 ****************************************************************************/

int nfs_rename(struct inode *mountpt, const char *oldrelpath,
                          const char *newrelpath)
{
  struct nsfmount *nmp;
  struct nfsnode *np;
  void *datreply;
  struct RENAME3args *rename;
  struct RENAME3resok resok;
  int error = 0;

  /* Sanity checks */

  DEBUGASSERT(mountpt && mountpt->i_private);

  /* Get the mountpoint private data from the inode structure */

  nmp = (struct nfsmount *)mountpt->i_private;
  np = nmp->nm_head;

  /* Check if the mount is still healthy */

  nfs_semtake(nmp);
  error = nfs_checkmount(nmp);
  if (error != 0)
    {
      goto errout_with_semaphore;
    }

  if (np->nfsv3_type != NFREG && np->nfsv3_type != NFDIR)
    {
      fdbg("open eacces typ=%d\n", np->nfsv3_type);
      error= -EACCES;
      goto errout_with_semaphore;
    }

  nfsstats.rpccnt[NFSPROC_RENAME]++;
  rename->from->dir = nmp->nm_fh;
  rename->from->name = oldrelpath;
  rename->to->dir = nmp->nm_fh;
  rename->to->name = oldrelpath;

  error = nfs_request(fdvp, NFSPROC_RENAME, rename, datareply);

  /* Kludge: Map ENOENT => 0 assuming that it is a reply to a retry. */

  if (error == ENOENT)
    {
      error = 0;
    }

  if (error)
    {
       goto errout_with_semaphore;
    }

  resok = (struct RENAME3resok *) datareply;
  np->n_fattr = resok->todir_wcc->after
  np->n_flag |= NMODIFIED;
  NFS_INVALIDATE_ATTRCACHE(np);

errout_with_semaphore:
   nfs_semgive(nmp);
   return error;
}

/****************************************************************************
 * Name: nfs_mkdir
 *
 * Description: Create a directory
 *
 ****************************************************************************/

int nfs_mkdir(struct inode *mountpt, const char *relpath, mode_t mode)
{
  struct nfsv3_sattr *vap;
  struct nfsv3_sattr *sp;
  struct nfsmount *nmp;
  struct nfsnode *np;
  struct MKDIR3args *mkdir;
  struct MKDIR3resok resok;
  void *datareply;
  int len;
  struct nfsnode *npL;
  int error = 0;

  /* Sanity checks */

  DEBUGASSERT(mountpt && mountpt->i_private);

  /* Get the mountpoint private data from the inode structure */

  nmp = (struct nfsmount*) mountpt->i_private;
  np = nmp->nm_head;
  vap = np->n_fattr;

  /* Check if the mount is still healthy */

  nfs_semtake(nmp);
  error = nfs_checkmount(nmp);
  if (error != 0)
    {
      goto errout_with_semaphore;
    }

  nfsstats.rpccnt[NFSPROC_MKDIR]++;
  mkdir->dir = nmp->nm_fh;
  mkdir->name = relpath;

  sp->sa_modetrue = nfs_true;
  sp->sa_mode = txdr_unsigned(vap->sa_mode);
  sp->sa_uid = nfs_xdrneg1;
  sp->sa_gid = nfs_xdrneg1;
  sp->sa_size = nfs_xdrneg1;
  sp->sa_atimetype = txdr_unsigned(NFSV3SATTRTIME_TOCLIENT);
  sp->sa_mtimetype = txdr_unsigned(NFSV3SATTRTIME_TOCLIENT);

  txdr_nfsv3time(&vap->sa_atime, &sp->sa_atime);
  txdr_nfsv3time(&vap->sa_mtime, &sp->sa_mtime);

  mkdir->attributes = sp;

  error = nfs_request(nmp, NFSPROC_MKDIR, mkdir, datareply);
  if (error)
    {
      goto errout_with_semaphore;
    }

  nmp->n_flag |= NMODIFIED;
  NFS_INVALIDATE_ATTRCACHE(VTONFS(dvp));

errout_with_semaphore:
  nfs_semgive(nmp);
  return error;
}

/* nfs remove directory call */

int nfs_rmdir(void *v)
{
  struct vop_rmdir_args *ap = v;
  struct vnode *vp = ap->a_vp;
  struct vnode *dvp = ap->a_dvp;
  struct componentname *cnp = ap->a_cnp;
  struct nfsm_info info;
  uint32_t *tl;
  int32_t t1;
  caddr_t cp2;
  int error = 0, wccflag = NFSV3_WCCRATTR;

  info.nmi_v3 = NFS_ISV3(dvp);

  if (dvp == vp)
    {
      vrele(dvp);
      vrele(dvp);
      pool_put(&namei_pool, cnp->cn_pnbuf);
      return EINVAL;
    }

  nfsstats.rpccnt[NFSPROC_RMDIR]++;
  info.nmi_mb = info.nmi_mreq = nfsm_reqhead(NFSX_FH(info.nmi_v3) +
                                             NFSX_UNSIGNED +
                                             nfsm_rndup(cnp->cn_namelen));
  nfsm_fhtom(&info, dvp, info.nmi_v3);
  nfsm_strtom(cnp->cn_nameptr, cnp->cn_namelen, NFS_MAXNAMLEN);

  info.nmi_procp = cnp->cn_proc;
  info.nmi_cred = cnp->cn_cred;
  error = nfs_request(dvp, NFSPROC_RMDIR, &info);
  if (info.nmi_v3)
    {
      nfsm_wcc_data(dvp, wccflag);
    }

  m_freem(info.nmi_mrep);

nfsmout:
  pool_put(&namei_pool, cnp->cn_pnbuf);
  VTONFS(dvp)->n_flag |= NMODIFIED;
  if (!wccflag)
    {
      NFS_INVALIDATE_ATTRCACHE(VTONFS(dvp));
    }

  VN_KNOTE(dvp, NOTE_WRITE | NOTE_LINK);
  VN_KNOTE(vp, NOTE_DELETE);

  cache_purge(vp);
  vrele(vp);
  vrele(dvp);

  /* Kludge: Map ENOENT => 0 assuming that you have a reply to a retry. */

  if (error == ENOENT)
    {
      error = 0;
    }

  return error;
}

/* The readdir logic below has a big design bug. It stores the NFS cookie in
 * the returned uio->uio_offset but does not store the verifier (it cannot).
 * Instead, the code stores the verifier in the nfsnode and applies that
 * verifies to all cookies, no matter what verifier was originally with
 * the cookie.
 *
 * From a practical standpoint, this is not a problem since almost all
 * NFS servers do not change the validity of cookies across deletes
 * and inserts.
 */

/* nfs readdir call */

int nfs_readdir(struct inode *mountpt, struct fs_dirent_s *dir) //seems done
{
  //struct nfsnode *np = VTONFS(vp);
  int error = 0;
  unsigned long *cookies = NULL;
  int cnt;
  struct nfsmount *nmp;
  struct nfsnode *np;
  int eof = 0;
  //struct nfs_dirent *ndp;

  fvdbg("Entry\n");

  /* Sanity checks */

  DEBUGASSERT(mountpt != NULL && mountpt->i_private != NULL);

  /* Recover our private data from the inode instance */

  nmp = mountpt->i_private;
  np  = np->nm_head;

  /* Make sure that the mount is still healthy */

  nfs_semtake(nmp);
  error = nfs_checkmount(nmp);
  if (error != 0)
    {
      fdbg("romfs_checkmount failed: %d\n", error);
      goto errout_with_semaphore;
    }

  if (np->nfsv3_type != NFDIR)
    {
      error = EPERM;
      goto errout_with_semaphore;
    }

  dir->u.nfs.nd_direoffset = np->nd_direoffset;

  /* First, check for hit on the EOF offset */

  if (dir->u.nfs.nd_direoffset != 0)
    {
      nfsstats.direofcache_hits++;
      //np->n_open = true;
      return 0;
    }

  if ((nmp->nm_flag & (NFSMNT_NFSV3 | NFSMNT_GOTFSINFO)) == NFSMNT_NFSV3)
    {
      (void)nfs_fsinfo(nmp);
    }
  cnt = 5;

  do
    {
      error = nfs_readdirrpc(nmp, &eof, dir);

      if (error == NFSERR_BAD_COOKIE)
        {
          error = EINVAL;
        }
    }
  while (!error && !eof && cnt--);

  if (!error && eof)
    {
      nfsstats.direofcache_misses++;
      nfs_semgive(nmp);
      return 0;
    }

errout_with_semaphore:
  nfs_semgive(nmp);
  return error;
}

/* The function below stuff the cookies in after the name */

/* Readdir rpc call. */

int nfs_readdirrpc(struct nfsmount *nmp, int *end_of_directory, fs_dirent_s *dir) //seems done
{
  int len, left;
  struct nfs_dirent *ndp = NULL;
  struct nfs_dirent *dp = NULL;
  nfsuint64 cookie;
  struct nfsnode *dnp = nmp->nm_head;
  int error = 0, more_dirs = 1, blksiz = 0, bigenough = 1;
  int attrflag;
  int info_v3;
  void *datareply;

  info_v3 = (nmp->nm_flag & NFSMNT_NFSV3);

  /* Loop around doing readdir rpc's of size nm_readdirsize
   * truncated to a multiple of NFS_READDIRBLKSIZ.
   * The stopping criteria is EOF or buffer full.
   */

  while (more_dirs && bigenough)
    {
      nfsstats.rpccnt[NFSPROC_READDIR]++;
      if (info_v3)
        {
          cookie.nfsuquad[0] = dnp->n_cookieverf.nfsuquad[0];
          cookie.nfsuquad[1] = dnp->n_cookieverf.nfsuquad[1];
        }
      else
        {
          cookie.nfsuquad[1] = dnp->n_cookieverf.nfsuquad[1];
        }

      error = nfs_request(nmp, NFSPROC_READDIR, datareply);
      dp = (void nfs_dirent*) datareply;

      if (error)
        {
          goto nfsmout;
        }

      if (info_v3)
        {
          dnp->n_cookieverf.nfsuquad[0] = dp->cookie[0];
          dnp->n_cookieverf.nfsuquad[1] = dp->cookie[1];
        }

      more_dirs = fxdr_unsigned(int, *dp);

      /* loop thru the dir entries*/

      while (more_dirs && bigenough)
        {
          if (bigenough)
            {
              if (info_v3)
                {
                  dir->u.nfs.cookie[0] = cookie.nfsuquad[0];
                }
              else
                {
                  dir->u.nfs.cookie[0] = ndp->cookie[0] = 0;
                }

              dir->u.nfs.cookie[1] = ndp->cookie[1] = cookie.nfsuquad[1];
            }

          more_dirs = fxdr_unsigned(int, *ndp);
        }
    }

  /* We are now either at the end of the directory or have filled the
   * block.
   */

  if (bigenough)
    {
      dnp->n_direofoffset = fxdr_hyper(&cookie.nfsuquad[0]);
      if (end_of_directory)
        {
          *end_of_directory = 1;
        }

      /* We signal the end of the directory by returning the
       * special error -ENOENT
       */

      fdbg("End of directory\n");
      error = ENOENT;
    }

nfsmout:
  return error;
}

/****************************************************************************
 * Name: nfs_statfs
 *
 * Description: Return filesystem statistics
 *
 ****************************************************************************/

int nfs_statfs(struct inode *mountpt, struct statfs *sbp) //done
{
  struct nfs_statfs *sfp = NULL;
  struct nfsmount *nmp;
  int error = 0;
  uint64_t tquad;
  void *datareply;
  int info_v3;

  /* Sanity checks */

  DEBUGASSERT(mountpt && mountpt->i_private);

  /* Get the mountpoint private data from the inode structure */

  nmp = mountpt->i_private;
  info_v3 = (nmp->nm_flag & NFSMNT_NFSV3);

  /* Check if the mount is still healthy */

  nfs_semtake(nmp);
  error = nfs_checkmount(nmp);
  if (error < 0)
    {
      fdbg("romfs_checkmount failed: %d\n", error);
      goto errout_with_semaphore;
    }

  /* Fill in the statfs info */

  memset(sbp, 0, sizeof(struct statfs));
  sbp->f_type = NFS_SUPER_MAGIC;

  if (info_v3 && (nmp->nm_flag & NFSMNT_GOTFSINFO) == 0)
    {
      (void)nfs_fsinfo(nmp);
    }

  nfsstats.rpccnt[NFSPROC_FSSTAT]++;

  error = nfs_request(nmp, NFSPROC_FSSTAT, datareply);
  if (error)
    {
      goto errout_with_semaphore;
    }

  sfp = (struct nfs_statfs *)datareply;
  if (info_v3)
    {
      sbp->f_bsize = NFS_FABLKSIZE;
      tquad = fxdr_hyper(&sfp->sf_tbytes);
      sbp->f_blocks = tquad / (uint64_t) NFS_FABLKSIZE;
      tquad = fxdr_hyper(&sfp->sf_fbytes);
      sbp->f_bfree = tquad / (uint64_t) NFS_FABLKSIZE;
      tquad = fxdr_hyper(&sfp->sf_abytes);
      sbp->f_bavail = (quad_t) tquad / (quad_t) NFS_FABLKSIZE;

      tquad = fxdr_hyper(&sfp->sf_tfiles);
      sbp->f_files = tquad;
      tquad = fxdr_hyper(&sfp->sf_ffiles);
      sbp->f_ffree = tquad;
      sbp->f_namelen = MAXNAMLEN;
    }
  else
    {
      sbp->f_bsize = fxdr_unsigned(int32_t, sfp->sf_bsize);
      sbp->f_blocks = fxdr_unsigned(int32_t, sfp->sf_blocks);
      sbp->f_bfree = fxdr_unsigned(int32_t, sfp->sf_bfree);
      sbp->f_bavail = fxdr_unsigned(int32_t, sfp->sf_bavail);
      sbp->f_files = 0;
      sbp->f_ffree = 0;
    }

errout_with_semaphore:
  nfs_semgive(nmp);
  return error;
}

/* Print out the contents of an nfsnode. */
/*
int nfs_print(struct file *filep)
{
  //struct vnode *vp = ap->a_vp;
  struct nfsnode *np = VTONFS(filep);

  nvdbg("tag VT_NFS, fileid %ld fsid 0x%lx",
        np->n_fattr.nfsv3fa_fileid, np->n_fattr.nfsv3fa_fsid);
  nvdbg("\n");
  return 0;
}
*/
/* nfs version 3 fsinfo rpc call */

int nfs_fsinfo(struct nfsmount *nmp) //done
{
  struct nfsv3_fsinfo *fsp;
  uint32_t pref, max;
  int error = 0;
  void *datareply;

  nfsstats.rpccnt[NFSPROC_FSINFO]++;

  error = nfs_request(nmp, NFSPROC_FSINFO, datareply);
  if (error)
    {
      goto nfsmout;
    }

  fsp = (struct nfsv3_fsinfo *)datareply;
  pref = fxdr_unsigned(uint32_t, fsp->fs_wtpref);
  if (pref < nmp->nm_wsize)
    {
      nmp->nm_wsize = (pref + NFS_FABLKSIZE - 1) & ~(NFS_FABLKSIZE - 1);
    }

  max = fxdr_unsigned(uint32_t, fsp->fs_wtmax);
  if (max < nmp->nm_wsize)
    {
      nmp->nm_wsize = max & ~(NFS_FABLKSIZE - 1);
      if (nmp->nm_wsize == 0)
        nmp->nm_wsize = max;
    }

  pref = fxdr_unsigned(uint32_t, fsp->fs_rtpref);
  if (pref < nmp->nm_rsize)
    {
      nmp->nm_rsize = (pref + NFS_FABLKSIZE - 1) & ~(NFS_FABLKSIZE - 1);
    }

  max = fxdr_unsigned(uint32_t, fsp->fs_rtmax);
  if (max < nmp->nm_rsize)
    {
      nmp->nm_rsize = max & ~(NFS_FABLKSIZE - 1);
      if (nmp->nm_rsize == 0)
        {
          nmp->nm_rsize = max;
        }
    }

  pref = fxdr_unsigned(uint32_t, fsp->fs_dtpref);
  if (pref < nmp->nm_readdirsize)
    {
      nmp->nm_readdirsize = (pref + NFS_DIRBLKSIZ - 1) & ~(NFS_DIRBLKSIZ - 1);
    }

  if (max < nmp->nm_readdirsize)
    {
      nmp->nm_readdirsize = max & ~(NFS_DIRBLKSIZ - 1);
      if (nmp->nm_readdirsize == 0)
        {
          nmp->nm_readdirsize = max;
        }
    }

  nmp->nm_flag |= NFSMNT_GOTFSINFO;

nfsmout:
  return error;
}

void nfs_decode_args(struct nfsmount *nmp, struct nfs_args *argp) //done
{
  int adjsock = 0;
  int maxio;

#ifdef CONFIG_NFS_TCPIP
  /* Re-bind if rsrvd port requested and wasn't on one */

  adjsock = !(nmp->nm_flag & NFSMNT_RESVPORT)
    && (argp->flags & NFSMNT_RESVPORT);
#endif

  /* Also re-bind if we're switching to/from a connected UDP socket */

  adjsock |= ((nmp->nm_flag & NFSMNT_NOCONN) != (argp->flags & NFSMNT_NOCONN));

  /* Update flags atomically.  Don't change the lock bits. */

  nmp->nm_flag =
    (argp->flags & ~NFSMNT_INTERNAL) | (nmp->nm_flag & NFSMNT_INTERNAL);

  if ((argp->flags & NFSMNT_TIMEO) && argp->timeo > 0)
    {
      nmp->nm_timeo = (argp->timeo * NFS_HZ + 5) / 10;
      if (nmp->nm_timeo < NFS_MINTIMEO)
        {
          nmp->nm_timeo = NFS_MINTIMEO;
        }
      else if (nmp->nm_timeo > NFS_MAXTIMEO)
        {
          nmp->nm_timeo = NFS_MAXTIMEO;
        }
    }

  if ((argp->flags & NFSMNT_RETRANS) && argp->retrans > 1)
    {
      nmp->nm_retry = MIN(argp->retrans, NFS_MAXREXMIT);
    }

  if (!(nmp->nm_flag & NFSMNT_SOFT))
    {
      nmp->nm_retry = NFS_MAXREXMIT + 1;  /* past clip limit */
    }

  if (argp->flags & NFSMNT_NFSV3)
    {
      if (argp->sotype == SOCK_DGRAM)
        {
          maxio = NFS_MAXDGRAMDATA;
        }
      else
        {
          maxio = NFS_MAXDATA;
        }
    }
  else
    {
      maxio = NFS_V2MAXDATA;
    }

  if ((argp->flags & NFSMNT_WSIZE) && argp->wsize > 0)
    {
      int osize = nmp->nm_wsize;
      nmp->nm_wsize = argp->wsize;

      /* Round down to multiple of blocksize */

      nmp->nm_wsize &= ~(NFS_FABLKSIZE - 1);
      if (nmp->nm_wsize <= 0)
        {
          nmp->nm_wsize = NFS_FABLKSIZE;
        }

      adjsock |= (nmp->nm_wsize != osize);
    }

  if (nmp->nm_wsize > maxio)
    {
      nmp->nm_wsize = maxio;
    }

  if (nmp->nm_wsize > MAXBSIZE)
    {
      nmp->nm_wsize = MAXBSIZE;
    }

  if ((argp->flags & NFSMNT_RSIZE) && argp->rsize > 0)
    {
      int osize = nmp->nm_rsize;
      nmp->nm_rsize = argp->rsize;

      /* Round down to multiple of blocksize */

      nmp->nm_rsize &= ~(NFS_FABLKSIZE - 1);
      if (nmp->nm_rsize <= 0)
        {
          nmp->nm_rsize = NFS_FABLKSIZE;
        }

      adjsock |= (nmp->nm_rsize != osize);
    }

  if (nmp->nm_rsize > maxio)
    {
      nmp->nm_rsize = maxio;
    }

  if (nmp->nm_rsize > MAXBSIZE)
    {
      nmp->nm_rsize = MAXBSIZE;
    }

  if ((argp->flags & NFSMNT_READDIRSIZE) && argp->readdirsize > 0)
    {
      nmp->nm_readdirsize = argp->readdirsize;

      /* Round down to multiple of blocksize */

      nmp->nm_readdirsize &= ~(NFS_DIRBLKSIZ - 1);
      if (nmp->nm_readdirsize < NFS_DIRBLKSIZ)
        {
          nmp->nm_readdirsize = NFS_DIRBLKSIZ;
        }
    }
  else if (argp->flags & NFSMNT_RSIZE)
    {
      nmp->nm_readdirsize = nmp->nm_rsize;
    }

  if (nmp->nm_readdirsize > maxio)
    {
      nmp->nm_readdirsize = maxio;
    }

  if ((argp->flags & NFSMNT_MAXGRPS) && argp->maxgrouplist >= 0 &&
      argp->maxgrouplist <= NFS_MAXGRPS)
    {
      nmp->nm_numgrps = argp->maxgrouplist;
    }

  if ((argp->flags & NFSMNT_READAHEAD) && argp->readahead >= 0 &&
      argp->readahead <= NFS_MAXRAHEAD)
    {
      nmp->nm_readahead = argp->readahead;
    }

  if (argp->flags & NFSMNT_ACREGMIN && argp->acregmin >= 0)
    {
      if (argp->acregmin > 0xffff)
        {
          nmp->nm_acregmin = 0xffff;
        }
      else
        {
          nmp->nm_acregmin = argp->acregmin;
        }
    }

  if (argp->flags & NFSMNT_ACREGMAX && argp->acregmax >= 0)
    {
      if (argp->acregmax > 0xffff)
        {
          nmp->nm_acregmax = 0xffff;
        }
      else
        {
          nmp->nm_acregmax = argp->acregmax;
        }
    }

  if (nmp->nm_acregmin > nmp->nm_acregmax)
    {
      nmp->nm_acregmin = nmp->nm_acregmax;
    }

  if (argp->flags & NFSMNT_ACDIRMIN && argp->acdirmin >= 0)
    {
      if (argp->acdirmin > 0xffff)
        {
          nmp->nm_acdirmin = 0xffff;
        }
      else
        {
          nmp->nm_acdirmin = argp->acdirmin;
        }
    }

  if (argp->flags & NFSMNT_ACDIRMAX && argp->acdirmax >= 0)
    {
      if (argp->acdirmax > 0xffff)
        {
          nmp->nm_acdirmax = 0xffff;
        }
      else
        {
          nmp->nm_acdirmax = argp->acdirmax;
        }
    }

  if (nmp->nm_acdirmin > nmp->nm_acdirmax)
    {
      nmp->nm_acdirmin = nmp->nm_acdirmax;
    }

  if (nmp->nm_so && adjsock)
    {
      nfs_disconnect(nmp);
      if (nmp->nm_sotype == SOCK_DGRAM)
        while (nfs_connect(nmp))
          {
            nvdbg("nfs_args: retrying connect\n");
          }
    }
}

/****************************************************************************
 * Name: nfs_mount
 *
 * Description: This implements a portion of the mount operation. This
 *  function allocates and initializes the mountpoint private data and
 *  binds the blockdriver inode to the filesystem private data.  The final
 *  binding of the private data (containing the blockdriver) to the
 *  mountpoint is performed by mount().
 *
 ****************************************************************************/

int nfs_mount(struct inode *blkdriver, void *data, void **handle) //done
{
  int error;
  struct nfs_args args;
  struct sockaddr *nam;
  nfsfh_t nfh[NFSX_V3FHMAX];

  bcopy(data, &args, sizeof(args.version));
  if (args.version == 3)
    {
      bcopy(data, &args, sizeof(struct nfs_args3));
      args.flags &= ~(NFSMNT_INTERNAL | NFSMNT_NOAC);
    }
  else if (args.version == NFS_ARGSVERSION)
    {
      error = copyin(data, &args, sizeof(struct nfs_args));
      args.flags &= ~NFSMNT_NOAC;
    }
  else
    {
      return EPROGMISMATCH;
    }

  if ((args.flags & (NFSMNT_NFSV3 | NFSMNT_RDIRPLUS)) == NFSMNT_RDIRPLUS)
    {
      return EINVAL;
    }

  if (blkdriver->mnt_flag & MNT_UPDATE)
    {
      struct nfsmount *nmp = (struct nfsmount*)blkdriver->i_private;

      if (nmp == NULL)
        {
          return EIO;
        }

      /* When doing an update, we can't change from or to v3. */

      args.flags = (args.flags & ~(NFSMNT_NFSV3)) |
        (nmp->nm_flag & (NFSMNT_NFSV3));
      nfs_decode_args(nmp, &args);
      return 0;
    }

  if (args.fhsize < 0 || args.fhsize > NFSX_V3FHMAX)
    {
      return EINVAL;
    }

  bcopy(args.fh, nfh, args.fhsize);
  bcopy(args.addr, nam, sizeof(args.addr));
  args.fh = nfh;
  error = mountnfs(&args, blkdriver, nam);
  return error;
}

/* Common code for nfs_mount */

int mountnfs(struct nfs_args *argp, struct inode *blkdriver,
             struct sockaddr *nam, void **handle) //done
{
  struct nfsmount *nmp;
  int error;

  if (blkdriver->mnt_flag & MNT_UPDATE)
    {
      nmp = (struct nfsmount*)blkdriver->i_private;

      /* update paths, file handles, etc, here XXX */

      return 0;
    }
  else
    {
      /* Open the block driver */

      if (!blkdriver || !blkdriver->u.i_bops)
        {
          fdbg("No block driver/ops\n");
          return -ENODEV;
        }

      if (blkdriver->u.i_bops->open &&
          blkdriver->u.i_bops->open(blkdriver) != OK)
        {
          fdbg("No open method\n");
          return -ENODEV;
        }

      /* Create an instance of the mountpt state structure */

      nmp = (struct nfsmount*)zalloc(sizeof(struct nfmount));
      if (!nmp)
        {
          fdbg("Failed to allocate mountpoint structure\n");
          return -ENOMEM;
        }

      /* Initialize the allocated mountpt state structure.  The filesystem is
       * responsible for one reference ont the blkdriver inode and does not
       * have to addref() here (but does have to release in ubind().
       */

      sem_init(&rm->rm_sem, 0, 0);     /* Initialize the semaphore that controls access */

    //vfs_getnewfsid(mp);
      nmp->nm_blkdriver = blkdriver;          /* Save the block driver reference */
      nmp->nm_timeo = NFS_TIMEO;
      nmp->nm_retry = NFS_RETRANS;
      nmp->nm_wsize = NFS_WSIZE;
      nmp->nm_rsize = NFS_RSIZE;
      nmp->nm_readdirsize = NFS_READDIRSIZE;
      nmp->nm_numgrps = NFS_MAXGRPS;
      nmp->nm_readahead = NFS_DEFRAHEAD;
      nmp->nm_fhsize = argp->fhsize;
      nmp->nm_acregmin = NFS_MINATTRTIMO;
      nmp->nm_acregmax = NFS_MAXATTRTIMO;
      nmp->nm_acdirmin = NFS_MINATTRTIMO;
      nmp->nm_acdirmax = NFS_MAXATTRTIMO;
      memmove(nmp->nm_fh, argp->fh, argp->fhsize);
    //strncpy(&mp->mnt_stat.f_fstypename[0], mp->mnt_vfc->vfc_name, MFSNAMELEN);
    //memmove(hst, mp->mnt_stat.f_mntfromname, MNAMELEN);
     // bcopy(pth, nmp->nm_mntonname, 90);
    //memmove(argp, &mp->mnt_stat.mount_info.nfs_args, sizeof(*argp));
      nmp->nm_nam = nam;
      nfs_decode_args(nmp, argp);

      /* Set up the sockets and per-host congestion */

      nmp->nm_sotype = argp->sotype;
      nmp->nm_soproto = argp->proto;

      /* For Connection based sockets (TCP,...) defer the connect until
       * the first request, in case the server is not responding.
       */

      if (nmp->nm_sotype == SOCK_DGRAM && (error = nfs_connect(nmp)))
        {
          goto bad;
        }

      /* Mounted! */

      nmp->nfs_mounted = true;
      nfs_init();
      *handle = blkdriver->i_private = &nmp;
      nfs_semgive(nmp);

      return 0;
  }

bad:
  nfs_disconnect(nmp);
  sem_destroy(&nmp->nm_sem);
  kfree(nmp);
  return error;
}

/****************************************************************************
 * Name: nfs_unmount
 *
 * Description: This implements the filesystem portion of the umount
 *   operation.
 *
 ****************************************************************************/

int nfs_unmount(struct inode *blkdriver, void *handle) //done
{
  struct nfsmount *nmp = (struct nfsmount*) handle ;
  int error;

  fvdbg("Entry\n");

  if (!nmp)
    {
      return -EINVAL;
    }

  nfs_semtake(nmp)
  if (nmp->nm_head)
    {
      /* We cannot unmount now.. there are open files */

      error = -EBUSY;
    }
  else
    {
       /* Unmount ... close the block driver */

      if (nmp->nm_blkdriver)
        {
          struct inode *inode = nmp->nm_blkdriver;
          if (inode)
            {
              if (inode->u.i_bops && inode->u.i_bops->close)
                {
                  (void)inode->u.i_bops->close(inode);
                }

              /* We hold a reference to the block driver but should
               * not but mucking with inodes in this context.  So, we will just return
               * our contained reference to the block driver inode and let the umount
               * logic dispose of it.
               */

              if (blkdriver)
                {
                  *blkdriver = inode;
                }
            }
        }

      /* Release the mountpoint private data */

      if (nmp->nm_buffer)
        {
          kfree(nmp->nm_buffer);
        }

      nfs_disconnect(nmp);
      sem_destroy(&rm->rm_sem);
      kfree(nmp);

      return 0;
    }

  nfs_semgive(nmp)
  return 0;
}
/****************************************************************************
 * Name: nfs_sync
 *
 * Description: Flush out the buffer cache
 *
 ****************************************************************************/

int nfs_sync(struct file *filep) //falta
{
  struct inode *inode;
  struct nfsmount *nmp;
  struct nfsnode *np;
  int error = 0;

  /* Sanity checks */

  DEBUGASSERT(filep->f_priv != NULL && filep->f_inode != NULL);

  /* Recover our private data from the struct file instance */

  np    = filep->f_priv;
  inode = filep->f_inode;
  nmp   = inode->i_private;

  DEBUGASSERT(nmp != NULL);

  /* Make sure that the mount is still healthy */

  nfs_semtake(nmp);
  error = nfs_checkmount(nmp);
  if (error != 0)
    {
      goto errout_with_semaphore;
    }

  /* Force stale buffer cache information to be flushed. */

  /* Check if the has been modified in any way */

  if ((np->n_flag & NMODIFIED) != 0)
    {
      error = VOP_FSYNC(vp, cred, waitfor, p); ///////////////////////////////
    }

  return allerror;

errout_with_semaphore:
  nfs_semgive(nmp);
  return error;
}
