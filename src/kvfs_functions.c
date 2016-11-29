/*
  Key Value System
  Copyright (C) 2016 Hung-Wei Tseng, Ph.D. <hungwei_tseng@ncsu.edu>

  This program can be distributed under the terms of the GNU GPLv3.
  See the file COPYING.

  This code is derived from function prototypes found /usr/include/fuse/fuse.h
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>
  His code is licensed under the LGPLv2.
  A copy of that code is included in the file fuse.h
  
  The point of this FUSE filesystem is to provide an introduction to
  FUSE.  It was my first FUSE filesystem as I got to know the
  software; hopefully, the comments in this code will help people who
  follow later to get a gentler introduction.


  Team Member:
  
  HengJin Tan 	: htan5
  Jingjuan Deng	: jdeng8
  Shanchuan Xia	: sxia4


*/

#include "kvfs.h"


//get full path
void get_fpath(char fp[PATH_MAX],char *path)
{
    char *root_path="/";
    root_path = str2md5(root_path, strlen(root_path));


    if(strcmp(path,root_path)==0)
    {
       printf("Outside Root!!!%s\n",path);
       strcpy(path,"/");
      strcpy(fp, KVFS_DATA->rootdir);
      strncat(fp, path, PATH_MAX); 
    }
    else
    {
      printf("INside Root!!!%s\n",path);
      strcpy(fp, KVFS_DATA->rootdir);
      strncat(fp, "/", PATH_MAX);
      strncat(fp, path, PATH_MAX);
    } 

    

}
///////////////////////////////////////////////////////////
//
// Prototypes for all these functions, and the C-style comments,
// come from /usr/include/fuse.h
//
/** Get file attributes.
 *
 * Similar to stat().  The 'st_dev' and 'st_blksize' fields are
 * ignored.  The 'st_ino' field is ignored except if the 'use_ino'
 * mount option is given.
 */
int kvfs_getattr_impl(const char *path, struct stat *stbuf)
{
printf("call getattr\n");
    char fpath[PATH_MAX];
    get_fpath(fpath,path);
    int ret;
    printf("Path ===> %s\n",path);
        ret=stat(fpath,stbuf);
        printf("FPATH ===> %s\n",fpath);
        if(ret)
        {
          printf("path not exits\n");
          printf("dir:%s\n",path);
          return -ENOENT;
        }  
        
    
    return ret;

}

/** Read the target of a symbolic link
 *
 * The buffer should be filled with a null terminated string.  The
 * buffer size argument includes the space for the terminating
 * null character.  If the linkname is too long to fit in the
 * buffer, it should be truncated.  The return value should be 0
 * for success.
 */
// Note the system readlink() will truncate and lose the terminating
// null.  So, the size passed to to the system readlink() must be one
// less than the size passed to kvfs_readlink()
// kvfs_readlink() code by Bernardo F Costa (thanks!)


int kvfs_readlink_impl(const char *path, char *link, size_t size)
{
printf("call readlink\n");
    int retstat;
    char fpath[PATH_MAX];

    get_fpath(fpath,path);

    retstat = readlink(fpath, link, size - 1);
    if (retstat >= 0) {
      link[retstat] = '\0';
      retstat = 0;
    }
    
    return retstat;
}

/** Create a file node
 *
 * There is no create() operation, mknod() will be called for
 * creation of all non-directory, non-symlink nodes.
 */
// shouldn't that comment be "if" there is no.... ?
int kvfs_mknod_impl(const char *path, mode_t mode, dev_t dev)
{
    printf("call mknod\n");
    int retstat;
    char fpath[PATH_MAX];
    get_fpath(fpath,path);
    if (S_ISREG(mode)) {
      retstat = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
    if (retstat >= 0)
      retstat = close(retstat);
    } else
    if (S_ISFIFO(mode))
      retstat = mkfifo(fpath, mode);
    else
      retstat =  mknod(fpath, mode, dev);
    
    return retstat;
}

/** Create a directory */
int kvfs_mkdir_impl(const char *path, mode_t mode)
{
    printf("call mkdir\n");
    int retstat;
    char fpath[PATH_MAX];
    get_fpath(fpath,path);
    printf("%s\n",path);
    printf("%s\n",fpath);
    printf("%s\n",KVFS_DATA->rootdir);

    retstat=mkdir(fpath, mode);
    //struct stat *stbuf;
    printf("%d\n",retstat);
    return retstat;
}

/** Remove a file */
int kvfs_unlink_impl(const char *path)
{
printf("call unlink\n");
    char fpath[PATH_MAX];
    get_fpath(fpath,path);

    return unlink(fpath);
}

/** Remove a directory */
int kvfs_rmdir_impl(const char *path)
{
printf("call rmdir\n");
    char fpath[PATH_MAX];
    get_fpath(fpath,path);

    return -rmdir(fpath);
}

/** Create a symbolic link */
// The parameters here are a little bit confusing, but do correspond
// to the symlink() system call.  The 'path' is where the link points,
// while the 'link' is the link itself.  So we need to leave the path
// unaltered, but insert the link into the mounted directory.

int kvfs_symlink_impl(const char *path, const char *link)
{
    printf("call symlink\n");
    char flink[PATH_MAX];
    get_fpath(flink,path);
    return symlink(path, flink);
}

/** Rename a file */
// both path and newpath are fs-relative
int kvfs_rename_impl(const char *path, const char *newpath)
{
    printf("call rename\n");
    char fpath[PATH_MAX];
    char fnewpath[PATH_MAX];
    get_fpath(fpath,path);
    get_fpath(fnewpath,newpath);
    return rename(fpath, fnewpath);
}

/** Create a hard link to a file */
int kvfs_link_impl(const char *path, const char *newpath)
{
    printf("call link\n");
    char fpath[PATH_MAX];
    char fnewpath[PATH_MAX];
    get_fpath(fpath,path);
    get_fpath(fnewpath,newpath);
    return link(fpath, fnewpath);
}

/** Change the permission bits of a file */
int kvfs_chmod_impl(const char *path, mode_t mode)
{
printf("call chmod\n");
    char fpath[PATH_MAX];
    get_fpath(fpath,path);

    int res;

	res = chmod(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;

}

/** Change the owner and group of a file */
int kvfs_chown_impl(const char *path, uid_t uid, gid_t gid)
{
printf("call chown\n");
    char fpath[PATH_MAX];
    get_fpath(fpath,path);
    return lchown(fpath, uid, gid);
}

/** Change the size of a file */
int kvfs_truncate_impl(const char *path, off_t newsize)
{
printf("call truncate\n");
    char fpath[PATH_MAX];
    get_fpath(fpath,path);
    return truncate(fpath, newsize);
}

/** Change the access and/or modification times of a file */
/* note -- I'll want to change this as soon as 2.6 is in debian testing */
int kvfs_utime_impl(const char *path, struct utimbuf *ubuf)
{
printf("call utime\n");
    char fpath[PATH_MAX];
    get_fpath(fpath,path);
    int retstat=utime(fpath, ubuf);
    printf("%d\n", ubuf->actime);
    return retstat;
}


/** File open operation
 *
 * No creation, or truncation flags (O_CREAT, O_EXCL, O_TRUNC)
 * will be passed to open().  Open should check if the operation
 * is permitted for the given flags.  Optionally open may also
 * return an arbitrary filehandle in the fuse_file_info structure,
 * which will be passed to all file operations.
 *
 * Changed in version 2.2
 */
int kvfs_open_impl(const char *path, struct fuse_file_info *fi)
{
  printf("CALL OPEN \n");

    char fpath[PATH_MAX];
    get_fpath(fpath,path);
	int res;

	res = open(fpath, fi->flags);
	if (res == -1)
		return -errno;

	fi->fh=res;
	return 0;
}

/** Read data from an open file
 *
 * Read should return exactly the number of bytes requested except
 * on EOF or error, otherwise the rest of the data will be
 * substituted with zeroes.  An exception to this is when the
 * 'direct_io' mount option is specified, in which case the return
 * value of the read system call will reflect the return value of
 * this operation.
 *
 * Changed in version 2.2
 */
// I don't fully understand the documentation above -- it doesn't
// match the documentation for the read() system call which says it
// can return with anything up to the amount of data requested. nor
// with the fusexmp code which returns the amount of data also
// returned by read.
int kvfs_read_impl(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	printf("call read\n");
	int res;
	res = pread(fi->fh, buf, size, offset);
	if (res == -1)
		res = -errno;

	// close(fd);
	return res;

}

/** Write data to an open file
 *
 * Write should return exactly the number of bytes requested
 * except on error.  An exception to this is when the 'direct_io'
 * mount option is specified (see read operation).
 *
 * Changed in version 2.2
 */
// As  with read(), the documentation above is inconsistent with the
// documentation for the write() system call.
int kvfs_write_impl(const char *path, const char *buf, size_t size, off_t offset,
	     struct fuse_file_info *fi)
{
printf("call write\n");

	int res;
	res = pwrite(fi->fh, buf, size, offset);
	if (res == -1)
		res = -errno;

	// close(fd);
	return res;
}

/** Get file system statistics
 *
 * The 'f_frsize', 'f_favail', 'f_fsid' and 'f_flag' fields are ignored
 *
 * Replaced 'struct statfs' parameter with 'struct statvfs' in
 * version 2.5
 */
int kvfs_statfs_impl(const char *path, struct statvfs *statv)
{
printf("call statfs\n");
    int retstat = 0;
    char fpath[PATH_MAX];
    get_fpath(fpath,path);

    return statvfs(fpath, statv);
}

/** Possibly flush cached data
 *
 * BIG NOTE: This is not equivalent to fsync().  It's not a
 * request to sync dirty data.
 *
 * Flush is called on each close() of a file descriptor.  So if a
 * filesystem wants to return write errors in close() and the file
 * has cached dirty data, this is a good place to write back data
 * and return any errors.  Since many applications ignore close()
 * errors this is not always useful.
 *
 * NOTE: The flush() method may be called more than once for each
 * open().  This happens if more than one file descriptor refers
 * to an opened file due to dup(), dup2() or fork() calls.  It is
 * not possible to determine if a flush is final, so each flush
 * should be treated equally.  Multiple write-flush sequences are
 * relatively rare, so this shouldn't be a problem.
 *
 * Filesystems shouldn't assume that flush will always be called
 * after some writes, or that if will be called at all.
 *
 * Changed in version 2.2
 */

// this is a no-op in BBFS.  It just logs the call and returns success
int kvfs_flush_impl(const char *path, struct fuse_file_info *fi)
{
printf("call flush\n");
    log_msg("\nkvfs_flush(path=\"%s\", fi=0x%08x)\n", path, fi);
    // no need to get fpath on this one, since I work from fi->fh not the path
    log_fi(fi);
	
    return 0;
}

/** Release an open file
 *
 * Release is called when there are no more references to an open
 * file: all file descriptors are closed and all memory mappings
 * are unmapped.
 *
 * For every open() call there will be exactly one release() call
 * with the same flags and file descriptor.  It is possible to
 * have a file opened more than once, in which case only the last
 * release will mean, that no more reads/writes will happen on the
 * file.  The return value of release is ignored.
 *
 * Changed in version 2.2
 */
int kvfs_release_impl(const char *path, struct fuse_file_info *fi)
{
printf("call release\n");
    return close(fi->fh);
}


/** Synchronize file contents
 *
 * If the datasync parameter is non-zero, then only the user data
 * should be flushed, not the meta data.
 *
 * Changed in version 2.2
 */


int kvfs_fsync_impl(const char *path, int datasync, struct fuse_file_info *fi)
{
printf("call fsync\n");
    #ifdef HAVE_FDATASYNC
      if (datasync)
        return fdatasync(fi->fh);
      else
    #endif  
    return fsync(fi->fh);
}

#ifdef HAVE_SYS_XATTR_H
/** Set extended attributes */
int kvfs_setxattr_impl(const char *path, const char *name, const char *value, size_t size, int flags)
{
printf("call setattr\n");
    char fpath[PATH_MAX];
    get_fpath(fpath,path);
    return lsetxattr(fpath, name, value, size, flags);
// retrun 0;
}

/** Get extended attributes */
int kvfs_getxattr_impl(const char *path, const char *name, char *value, size_t size)
{
  printf("call getxattr\n");

    int retstat = 0;

    char fpath[PATH_MAX];
    get_fpath(fpath,path);

    retstat = lgetxattr(fpath, name, value, size);
    return retstat;
}

/** List extended attributes */
int kvfs_listxattr_impl(const char *path, char *list, size_t size)
{
printf("call listattr\n");
    int retstat = 0;

    char fpath[PATH_MAX];
    get_fpath(fpath,path);

    return     retstat=llistxattr(fpath, list, size);;
}

/** Remove extended attributes */
int kvfs_removexattr_impl(const char *path, const char *name)
{
printf("call removeattr\n");
    char fpath[PATH_MAX];
    get_fpath(fpath,path);
    return lremovexattr(fpath, name);
}
#endif

/** Open directory
 *
 * This method should check if the open operation is permitted for
 * this  directory
 *
 * Introduced in version 2.3
 */
int kvfs_opendir_impl(const char *path, struct fuse_file_info *fi)
{
  printf("CALL OPENDIR \n");

  
  DIR *dp;
  int retstat = 0;
  char fpath[PATH_MAX];
  get_fpath(fpath,path);

  dp = opendir(fpath);

  if (dp==NULL)
  {
  	printf("root error \n");
    return retstat;
  }

  fi->fh = (intptr_t) dp;
  return retstat;

}

/** Read directory
 *
 * This supersedes the old getdir() interface.  New applications
 * should use this.
 *
 * The filesystem may choose between two modes of operation:
 *
 * 1) The readdir implementation ignores the offset parameter, and
 * passes zero to the filler function's offset.  The filler
 * function will not return '1' (unless an error happens), so the
 * whole directory is read in a single readdir operation.  This
 * works just like the old getdir() method.
 *
 * 2) The readdir implementation keeps track of the offsets of the
 * directory entries.  It uses the offset parameter and always
 * passes non-zero offset to the filler function.  When the buffer
 * is full (or an error happens) the filler function will return
 * '1'.
 *
 * Introduced in version 2.3
 */


// TOBE TEST
int kvfs_readdir_impl(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset,
	       struct fuse_file_info *fi)
{
printf("call readdir\n");
 int retstat = 0;
    DIR *dp;
    struct dirent *de;
   
    dp = (DIR *) (uintptr_t) fi->fh;

    de = readdir(dp);

    if (de == 0) {
	return retstat;
    }

    do {
	if (filler(buf, de->d_name, NULL, 0) != 0) {
		printf("error\n");
	    return -ENOMEM;
	}
    } while ((de = readdir(dp)) != NULL);
    
    return retstat;

}

/** Release directory
 *
 * Introduced in version 2.3
 */

int kvfs_releasedir_impl(const char *path, struct fuse_file_info *fi)
{
    int retstat = 0;
    closedir((DIR *) (uintptr_t) fi->fh);
    
    return retstat;
}

/** Synchronize directory contents
 *
 * If the datasync parameter is non-zero, then only the user data
 * should be flushed, not the meta data
 *
 * Introduced in version 2.3
 */
// when exactly is this called?  when a user calls fsync and it
// happens to be a directory? ??? >>> I need to implement this...
int kvfs_fsyncdir_impl(const char *path, int datasync, struct fuse_file_info *fi)
{
printf("call fsyncdir\n");
    return -1;
}



int kvfs_access_impl(const char *path, int mask)
{
  printf("CALL ACCESS \n");
  int retstat = 0;
  char fpath[PATH_MAX];
  get_fpath(fpath,path);
  printf("ACCESS FPATH ===> %s\n", fpath );
  retstat = access(fpath, mask);

    return retstat;
}

/**
 * Create and open a file
 *
 * If the file does not exist, first create it with the specified
 * mode, and then open it.
 *
 * If this method is not implemented or under Linux kernel
 * versions earlier than 2.6.15, the mknod() and open() methods
 * will be called instead.
 *
 * Introduced in version 2.5
 */
// Not implemented.  I had a version that used creat() to create and
// open the file, which it turned out opened the file write-only.

/**
 * Change the size of an open file
 *
 * This method is called instead of the truncate() method if the
 * truncation was invoked from an ftruncate() system call.
 *
 * If this method is not implemented or under Linux kernel
 * versions earlier than 2.6.15, the truncate() method will be
 * called instead.
 *
 * Introduced in version 2.5
 */
int kvfs_ftruncate_impl(const char *path, off_t offset, struct fuse_file_info *fi)
{
printf("call ftruncate\n");
    int retstat = 0;
    
    retstat = ftruncate(fi->fh, offset);
    return retstat;
}

/**
 * Get attributes from an open file
 *
 * This method is called instead of the getattr() method if the
 * file information is available.
 *
 * Currently this is only called after the create() method if that
 * is implemented (see above).  Later it may be called for
 * invocations of fstat() too.
 *
 * Introduced in version 2.5
 */
int kvfs_fgetattr_impl(const char *path, struct stat *statbuf, struct fuse_file_info *fi)
{
printf("call fgetattr\n");
    int retstat = 0;
    char fpath[PATH_MAX];
  	get_fpath(fpath,path);

  	return kvfs_getattr_impl(path, statbuf);
}

