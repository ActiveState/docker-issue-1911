#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#ifndef RTLD_NOW
#   define RTLD_NOW 1
#endif

#ifndef RTLD_LOCAL
#   define RTLD_LOCAL 0
#endif

typedef (*tf) (int);

char*
TclpTempFileName(void)
{
    static char fileName[L_tmpnam + 9];
    int fd;

    /*
     * We should also check against making more then TMP_MAX of these.
     */

    strcpy(fileName, "/tmp/tclXXXXXX");	/* INTL: Native. */
    fd = mkstemp(fileName);		/* INTL: Native. */
    if (fd == -1) {
      return NULL;
    }
    fcntl(fd, F_SETFD, FD_CLOEXEC);
    unlink(fileName);			/* INTL: Native. */

    close(fd);
    return fileName;
}

tf Load (char* path)
{
#define blen 8192
  static char buf [blen];
  int have, ifd, ofd;
  void* handle;
  char* tmppath;
  tf fun;

  if (1) {
    tmppath = TclpTempFileName();
    ofd = open (tmppath, O_RDWR|O_CREAT|O_EXCL, 0600);
    ifd = open (path,    O_RDONLY, 0);
    have = read (ifd, buf, blen);
    write (ofd, buf, have);
    close (ifd);
    close (ofd);
    chmod (tmppath, 0700);
  } else {
    tmppath = path;
  }

  handle = dlopen (tmppath, RTLD_NOW|RTLD_LOCAL);
  if (!handle) {
    fprintf (stderr,"%s missing\n", path);    
    exit (1);
  }

  fun = dlsym (handle, "fun");
  if (!fun) {
    fprintf (stderr,"%s fun missing\n", path);    
    exit (1);
  }
  unlink (tmppath);
  return fun;
}

int main ()
{
  void* handlea;
  void* handleb;
  tf funa;
  tf funb;

  funa = Load ("sha.so");
  funb = Load ("shb.so");

  funa (1);
  funb (1);
  return 0;
}
