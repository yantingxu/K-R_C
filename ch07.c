/*
 * Chapter 7: Standard Input and Output
 */

#include<stdio.h>       // /usr/include
#include<ctype.h>

// 7.1 putchar and getchar
// Text stream model: lines with '\n'
// int getchar(void) return EOF if reaching the end; <infile redirect the input stream, which is not included int argv
// int putchar(int) return EOF if error occurs; >outfile

void stream_tolower()
{
    int c;
    while ((c = getchar()) != EOF) {
        putchar(tolower(c));        // the functions are often just macros
    }
}


// 7.2 printf
// int printf(char *format, arg1, arg2, ...); return the number of chars printed
// format: %-(width).(precition)d
// width and precision could be paramed: printf("%.*s", max, s);
// :%-15.10s:    :hello, wor     :
// int sprintf(char *string, char *format, arg1, arg2, ...); save the formmated result into string, which must be large enough


// 7.3 Variable-len arglist
// The standard header <stdarg.h> contains a set of macro definitions that define how to step through an argument list.

#include<stdarg.h>
void miniprintf(char* fmt, ...)
{
    va_list ap;
    char *p, *sval;
    int ival;
    double dval;

    va_start(ap, fmt);      // init to the arg following fmt
    for (p = fmt; *p != '\0'; p++) {
        if (*p != '%') {
            putchar(*p);
            continue;
        }
        switch(*++p) {
            case 'd':
                ival = va_arg(ap, int);     // get next argument
                printf("%d", ival);
                break;
            case 'f':
                dval = va_arg(ap, double);
                printf("%f", dval);
                break;
            case 's':
                for (sval = va_arg(ap, char*); *sval != '\0'; sval++) {
                    putchar(*sval);
                }
                break;
            default:
                putchar(*p);
                break;
        }
    }
    va_end(ap);     // clean up
}


// 7.4 scanf
// int scanf(char* fmt, ...): match fmt with input and the matched parts are stored int the rest variables (must be pointer and not detected at compile-time), return the number of matches or EOF
// int sscanf(char* string, char* fmt, ...)
// Format: spaces, characters and conversion specifications, consisting of the character %, an optional assignment suppression character *, an optional number specifying a maximum field width, an optional h, l or L indicating the width of the target, and a conversion character. 
// An input field is defined as a string of non-white space characters;

void calculator()
{
    double i, sum;
    while (scanf("%lf", &i) == 1) {
        printf("\t%.2f", sum += i);
    }
}

void day_month_year()
{
    int day, year, month;
    char monthname[20];
    char line[1000];
    while (getline(line, sizeof(line)) > 0) {
        // scanf ignores blanks and tabs in its format string.
        // Furthermore, it skips over white space (blanks, tabs, newlines, etc.) as it looks for input values.
        if( scanf("%d %s %d", &day, monthname, &year) == 3 ) {
            printf("valid: %s", line);
        } else if ( scanf("%d/%d/%d", &day, &month, &year) == 3 ) {
            printf("valid: %s", line);
        } else {
            printf("invalid: %s", line);
        }
    }
}


// 7.5 File Access
// FILE *fp; where FILE is defined with typedef in stdio.h
// FILE *fopen(char* filename, char* mode); return NULL if error occurs
// model: r, w, a; b for binary files; r, a preserve old contents while w clean up it
//      int putc(int c, FILE *p);
//      int getc(FILE *p);
// stdin, stdout, stderr are opened by os, declared in stdio.h; they are constants
// putchar and getchar are special cases with stdin/out
//    #define getchar()    getc(stdin)
//    #define putchar(c)   putc((c), stdout)
// with files
//    int fscanf(FILE *fp, char *format, ...)
//    int fprintf(FILE *fp, char *format, ...)

int mycat(int argc, char* argv[])
{
    FILE *fp;
    void filecopy(FILE*, FILE*);
    char* prog = argv[0];

    if (argc == 1) {
        filecopy(stdin, stdout);
    } else {
        int i;
        for (i = 1; i < argc; i++) {
            if ((fp = fopen(argv[i], 'r')) == NULL) {
                fprintf(stderr, "Error: %s, %s", prog, argv[i]);
                exit(1);        // exit status is available to the caller process and before exit all file handles are closed
            } else {
                filecopy(fp, stdout);
                fclose(fp);     // flush the buffer before close
                // all the handles are auto-closed when the program exits
            }
        }
        if (ferror(stdout)) {
            fprintf(stderr, "Error: %s", prog);
            exit(2);
        }
    }
    exit(0);        // exit could be called from any function while return is exclusive to main()
    // return 0;
    // other functions: int ferror(FILE*); int feof(FILE*);
}

void filecopy(FILE* in, FILE* out)
{
    int c;
    while ((c = getc(in)) != EOF) {
        putc(c, out);
    }
}

// 7.7 Line Input/Output
// char* fgets(char* line, int maxline, FILE* fp);
// including the newline
// line[maxline-1] == '\0'
// return NULL if error/EOF
// int fputs(char* line, FILE* fp);
// return EOF if error
// gets and puts are special cases with stdin/out; but gets deletes '\n' while puts adds it

char* myfgets(char* line, int maxline, FILE* fp)
{
    register int c;
    register char* l;

    l = line;
    while (--maxline > 0 && (c = getc(fp)) != EOF) {
        if ((*l++ = c) == '\n')
            break;
    }
    *l = '\0';
    return (c == EOF && l == line) ? NULL : 0;
}

int myfputs(char* line, FILE* fp)
{
    int c;
    while (c = *line++) {
        putc(c, fp);
    }
    return ferror(fp) ? EOF : 0;
}

int mygetline(char* line, int max)
{
    if (myfgets(line, max, stdin) == NULL)
        return 0;
    else
        return strlen(line);
}

// 7.8 Miscellaneous Functions
// string.h: strcat, strncat; strcmp, strncmp; strcpy, strncpy; strchr, strrchr
// ctype.h: isalpha, isdigit, isalnum, isupper, islower, isspace, toupper, tolower
// int ungetc(int c, FILE* fp); return EOF if error occurs
// system(char* s) return system-dependent integer status
// math.h: sin, cos, atan2, exp, log, log10, pow, sqrt, fabs
// stdlib.h: #define frand() ((double) rand() / (RAND_MAX+1.0)) => [0.0, 1.0); srand(unsigned) for seeding
// storage management
//  void *malloc(size_t n)
//  void *calloc(size_t n, size_t size)
//it is a ghastly error to free something not obtained by calling malloc or calloc
/*
void free_list(node* head)
{
    node *p, *q;
    for (p = head; p != NULL; q = p) {
        q = p->next;
        free(p);
    }
}
*/

int main()
{
    return 0;
}


