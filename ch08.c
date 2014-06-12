/*
 * Chapter 8: The UNIX System Interface
 */

#include<stdio.h>
#include<unistd.h>

// system calls invoked from user program
// the building blocks for the library functions in Chapter 7

// 8.1 File Descriptor
// opening a file => check permission => fd(non-negative) => info maintained by os
// 0, 1, 2 => stdin, stdout, stderr; redirection is done by shell, not the program

// 8.2 Unbuffered I/O
// int n_read = read(int fd, char* buf, int n);
// return: n, 0(EOF), -1(Error)
// int n_write = write(int fd, char* buf, int n);
// return: n_write == n, n_write !=n

#define BUFSIZE 1024

void transfer(void)
{
    char buf[BUFSIZE];
    int n;
    while ((n = read(0, buf, BUFSIZE) == n))
        write(1, buf, BUFSIZE);
}

int mygetchar(void)
{
    char c;
    return (read(0, &c, 1) == 1) ? (unsigned char)c : EOF;
}

int mybuffered_getchar(void)
{
    static char buf[BUFSIZE];
    static char* bufp = buf;
    static int n = 0;

    if (n == 0) {
        n = read(0, buf, BUFSIZE);
        bufp = buf;
    }
    return (--n >= 0) ? (unsigned char)*bufp++ : EOF;
}

// 8.3 Open, Close, Create, Unlink
// int fd = open(char* name, int flags, int perms);
// return fd (fopen returns fp), -1 on ERROR
// flags: O_RDONLY, O_WRONLY, O_RDWR; perms = 0
// It is an error to open a file that does not exist.
// int fd = creat(char* name, int perms);
// return -1 on ERROR; rewrite previous contents
// perms: rwx

#include<fcntl.h>
#include<stdarg.h>
#define PERMS 0666

void mycp(int argc, char **argv)
{
    int fd1, fd2;
    if (argc != 3)
        myerror("Usage: cp from to")
    if ((fd1 = open(argv[1], O_RDONLY, 0)) == -1)
        myerror("Cannot Open: %s", argv[1]);
    if ((fd2 = creat(argv[2], PERMS)) == -1)
        myerror("Cannot Creat: %s", argv[2]);

    char *buf[BUFSIZE];
    int n;
    while ((n = read(fd1, buf, BUFSIZE)) > 0) {
        if (write(fd2, buf, n) != n)
            myerror("write error");
    }
    close(fd1);     // the same to fclose except that no buffer flush
    close(fd2);
}

void myerror(char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "error");
    vprintf(stderr, fmt, args);     // the same to fprintf but with initiailized args as the 2nd argument
    va_end(args);
    exit(1);
}

// unlink(char* name) => remove

// 8.4 Random Access-lseek
// long lseek(int fd, long offset, int origin);
// origin: 0, 1, 2 => beginning, current, end
// return current_position; -1 on ERROR
// similar: fseek with arugment of type FILE* and non-zero if error

int get(int fd, long, pos, char* buf, int n)
{
    if (lseek(fd, pos, 0) >= 0)
        return read(fd, buf, n);
    else
        return -1;
}

// 8.5 Implementation of fopen and getc
#define NULL      0
#define EOF       (-1)
#define BUFSIZ    1024
#define OPEN_MAX  20    /* max #files open at once */

typedef struct _iobuf {
   int  cnt;       /* characters left */
   char *ptr;      /* next character position */
   char *base;     /* location of buffer */
   int  flag;      /* mode of file access */
   int  fd;        /* file descriptor */
} FILE;

enum _flags {
   _READ   = 01,   /* file open for reading */
   _WRITE  = 02,   /* file open for writing */
   _UNBUF  = 04,   /* file is unbuffered */
   _EOF    = 010,  /* EOF has occurred on this file */
   _ERR    = 020   /* error occurred on this file */
};

FILE _iob[OPEN_MAX] = {    /* stdin, stdout, stderr */
   { 0, (char *) 0, (char *) 0, _READ, 0 },
   { 0, (char *) 0, (char *) 0, _WRITE, 1 },
   { 0, (char *) 0, (char *) 0, _WRITE, | _UNBUF, 2 }
};

#define stdin   (&_iob[0])
#define stdout  (&_iob[1])
#define stderr  (&_iob[2])

int _fillbuf(FILE *);
int _flushbuf(int, FILE *);

#define feof(p)     (((p)->flag & _EOF) != 0)
#define ferror(p)   (((p)->flag & _ERR) != 0)
#define fileno(p)   ((p)->fd)
#define getc(p)   (--(p)->cnt >= 0 ? (unsigned char) *(p)->ptr++ : _fillbuf(p))
#define putc(x,p) (--(p)->cnt >= 0 ? *(p)->ptr++ = (x) : _flushbuf((x),p))
#define getchar()   getc(stdin)
#define putchar(x)  putc((x), stdout)

int _fillbuf(FILE* p)
{
    if ((p->flag&(_READ|_EOF|_ERR)) != _READ)
        // 没有读取权限, 或者出错
        return EOF;

    // 更新base, ptr, cnt
    // 如果buf还没有alloc则请求内存 
    int bufsize = (p->flag&_UNBUF) ? 1 : BUFSIZE;
    if (p->base == NULL) {
        p->base = (char*)malloc(bufsize);
        if (p->base == NULL) {
            return EOF:
        }
    }
    p->ptr = p->base;
    p->cnt = read(p->fd, p->base, bufsize);
    if (p->cnt <= 0) {
        // 读取失败 
        if (p->cnt == 0) {
            p->flag |= _EOF;
        } else if (p->cnt < 0) {
            p->flag |= _ERR;
        }
        p->cnt = 0;
        return EOF;
    }
    return (unsigned char)*p->ptr++;
}

FILE* fopen(char* name, char* mode)
{
    if (*mode != 'r' && *mode != 'w' && *mode != 'a')
        return NULL;
    int i;
    for (i = 0; i < OPEN_MAX; i++) {
        if (_iob[i] & (_READ|_WRITE) == 0)
            break;
    }
    if (i == OPEN_MAX)
        return NULL;
    FILE* fp = _iob[i];

    int fd;
    if (*mode == 'r') {
        fd = open(name, O_RDONLY, 0);
    } else if (*model == 'w') {
        fd = creat(name, PERMS);
    } else {
        if ((fd = open(name, O_WRONLY, 0)) == -1) {
            fd = creat(name, PERMS);
        }
        lseek(fd, 0L, 2);
    }
    if (fd == -1) {
        return NULL;
    }
    fp->fd = fd;
    fp->base = NULL;
    fp->cnt = 0;
    fp->flag = (*model == 'r') ? _READ : _WRITE;
    return fp;
}

// 8.6 Listing Directories
// A directory is a file that contains a list of filenames and some indication of where they are located (index into inode list).
// The inode for a file is where all information about the file except its name is kept. A directory entry generally consists of only two items, the filename and an inode number. 

// Part 1: Stat-related
// sys/stat.h
int stat(char *, struct stat *);
struct stat
{
   dev_t     st_dev;      /* device of inode */
   ino_t     st_ino;      /* inode number */
   short     st_mode;     /* mode bits */
   short     st_nlink;    /* number of links to file */
   short     st_uid;      /* owners user id */
   short     st_gid;      /* owners group id */
   dev_t     st_rdev;     /* for special files */
   off_t     st_size;     /* file size in characters */
   time_t    st_atime;    /* time last accessed */
   time_t    st_mtime;    /* time last modified */
   time_t    st_ctime;    /* time originally created */
};
// st_mode: sys/types.h
#define S_IFMT    0160000  /* type of file: */
#define S_IFDIR   0040000  /* directory */
#define S_IFCHR   0020000  /* character special */
#define S_IFBLK   0060000  /* block special */
#define S_IFREG   0010000  /* regular */


// Part 2: Directory-related, dirent.h
#define NAME_MAX   14  /* longest filename component; */
                              /* system-dependent */
typedef struct {       /* portable directory entry */
   long ino;                  /* inode number */
   char name[NAME_MAX+1];     /* name + '\0' terminator */
} Dirent;

typedef struct {       /* minimal DIR: no buffering, etc. */
   int fd;               /* file descriptor for the directory */
   Dirent d;             /* the directory entry */
} DIR;

DIR *opendir(char *dirname);
Dirent *readdir(DIR *dfd);
void closedir(DIR *dfd);


// Part 3: System-independent functions
#include <stdio.h>
#include <string.h>
// #include "syscalls.h"
#include <fcntl.h>      /* flags for read and write */
#include <sys/types.h>  /* typedefs */
#include <sys/stat.h>   /* structure returned by stat */
// #include "dirent.h"

void fsize(char *)
/* print file name */
void shell(int argc, char **argv)
{
    if (argc == 1)
        fsize('.')
    else {
        while (--argc > 0) {
            fsize(*++argv);
        }
    }
}

int stat(char *, struct stat *);        // return inode or -1
void dirwalk(char *, void (*fcn)(char *));

/* fsize:  print the name of file "name" */
void fsize(char *name)
{
    struct stat statbuf;
    if (stat(name, &statbuf) == -1) {
        printf("error");
        return;
    }
    if (statbuf.st_mode & S_IFMT != S_IFDIR) {
        dirwalk(name, fsize);
    }
    printf("%d %s", statbuf.st_size, name);
}

#define MAX_PATH 1024
/*
DIR *opendir(char *dirname);
Dirent *readdir(DIR *dfd);
void closedir(DIR *dfd);
*/

/* dirwalk:  apply fcn to all files in dir */
void dirwalk(char *dir, void (*fcn)(char *))
{
    DIR* dfd;       // file descriptor
    Dirent* dp;
    if ((dfd = opendir(dir)) == NULL) {
        printf("error");
        return;
    }
    char name[MAX_PATH];
    while ((dp = readdir(dfd)) != NULL) {
        if (dp->name == "." || dp->name == "..") {
            continue;
        } else {
            sprintf(name, "%s/%s", dir, dp->name);
            (*fcn)(name);
        }
    }
    closedir(dfd);
}

// Part 4: System-dependent
// sys/dir.h
#ifndef DIRSIZ
#define DIRSIZ  14
#endif
struct direct {   /* directory entry */
   ino_t d_ino;           /* inode number */
   char  d_name[DIRSIZ];  /* long name does not have '\0' */
};

int fstat(int fd, struct stat *);       // the same as stat but with fd as arg instead of name

/* opendir:  open a directory for readdir calls */
DIR *opendir(char *dirname)
{
    DIR* dp;
    int fd = open(dirname, O_RDONLY, 0);
    if (fd == -1) {
        return NULL;
    }
    struct stat statbuf;
    if (fstat(fd, &statbuf) == -1) {
        return NULL;
    }
    if (statbuf.st_mode & S_IFMT != S_IFDIR) {
        return NULL;
    }
    dp = (DIR*)malloc(sizeof(DIR));
    if (dp == NULL) {
        return NULL;
    }
    dp->fd = fd;
    return dp;
}

void closedir(DIR* dp)
{
    if (dp) {
        close(dp->fd);
        free(dp);
    }
}

// an independent interface to a specific os
Dirent* readdir(DIR* dp)
{
    struct dirent dirbuf;   // local structure
    static Dirent d;        // portable structure

    int fd = dp->fd;
    while (read(fd, (char*)&dirbuf, sizeof(dirent)) == sizeof(dirent)) {
        if (dirbuf.d_ino == 0) {
            continue;
        }
        d.ino = dirbuf.d_ino;
        strncpy(d.name, dirbuf.d_name, DIRSIZE);
        d.name[DIRSIZE] = '\0';
        return &d;
    }
    return NULL;
}

// 8.7 Example: A storage allocator
// Thus its free storage is kept as a list of free blocks. Each block contains a size, a pointer to the next block, and the space itself. The blocks are kept in order of increasing storage address, and the last block (highest address) points to the first. 
// address alignment issues
// [on request]
// first-fit
// If the block is too big, it is split, and the proper amount is returned to the user while the residue remains on the free list. If no big-enough block is found, another large chunk is obtained by the operating system and linked into the free list. 
// [on free]
// Freeing also causes a search of the free list, to find the proper place to insert the block being freed.
// If the block being freed is adjacent to a free block on either side, it is coalesced with it into a single bigger block.

typedef long Align;    /* for alignment to long boundary */
union header {         /* block header */
   struct {
       union header *ptr; /* next block if on free list */
       unsigned size;     /* size of this block */
   } s;
   Align x;           /* force alignment of blocks */
};
typedef union header Header;

// the requested size in characters is rounded up to the proper number of header-sized units;
// one more unit for header
// returned pointer lies at the beginning of the free space, not header
// If a too-big block is found, the tail end is returned to the user; in this way the header of the original needs only to have its size adjusted.

static Header base;       /* empty list to get started */
static Header *freep = NULL;     /* start of free list */

/* malloc:  general-purpose storage allocator */
void *malloc(unsigned nbytes)
{
    Header *prev, *p;
    Header* morecore(unsigned);
    // 要分配的内存是Header的整数倍
    unsigned nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;     // why -1???

    // 首次调用时的初始化
    if (freep == NULL) {
        base.s.ptr = freep = prev = &base;
    } else {
        prev = freep;
    }

    for (p = prev->s.ptr;; p = prev, p = p->s.ptr) {
        if (p->s.size >= nunits) {
            if (p->s.size == nunits) {
                prev->s.ptr = p->s.ptr;
            } else if (p->s.size > nunits) {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prev;
            return (void*)(p+1);
        }
        if (p == freep) {
            if ((p = morecore(nunits)) == NULL)
                return NULL;
        }
    }
}

// we don't want to do that on every call to malloc, so morecore requests al least NALLOC units; this larger block will be chopped up as needed
// The UNIX system call sbrk(n) returns a pointer to n more bytes of storage. sbrk returns -1 if there was no space, even though NULL could have been a better design. The -1 must be cast to char * so it can be compared with the return value.
// portable only among machines for which general pointer comparison is meaningful. 
#define NALLOC 1024
Header* morecore(unsigned nu)
{
    char* sbrk(int);
    if (nu < NALLOC) {
        nu = NALLOC;
    }
    char* cp = sbrk(nu*sizeof(Header));
    if (cp == (char*)-1)
        return NULL;
    Header* up = (Header*)cp;
    free((void*)(up+1));
    return freep;
}

void free(void* ap)
{
    Header* bp = (Header*)(ap+1);
    Header* p;
    for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
        if (p >= p->s.ptr) {        // at the end of list
            break;
        }

    if (bp + bp->s.size == p->s.ptr) {
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else {
        bp->s.ptr = p->s.ptr;
    }

    if (p + p->s.size == bp) {
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else {
        p->s.ptr = bp;
    }
    freep = p;      // 如果bp和p合并了，那么外围的malloc还得再跑一圈; 没合并的话一步就找到了
}


int main()
{
    return 0;
}




