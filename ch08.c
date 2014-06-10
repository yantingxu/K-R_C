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
// The inode for a file is where all information about the file except its name is kept. A directory entry generally consists of only two items, the filename and an inode number. 


int main()
{
    return 0;
}



