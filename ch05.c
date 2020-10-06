#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>

// 5.1 Pointer and Address
// The & operator only applies to objects in memory: variables and array elements. It cannot be applied to expressions, constants, or register variables.
// ip = &z[0];       /* ip now points to z[0] */
// every pointer points to a specific data type. (There is one exception: a ``pointer to void'' is used to hold any type of pointer but cannot be dereferenced itself)
// ++*ip and (*ip)++, unary operators like * and ++ associate right to left
void func(char s[])
{
    s = s + 1;
    printf("%c", *s);
}

// 5.2 Points and Function Arguments
// Since C passes arguments to functions by value, there is no direct way for the called function to alter a variable in the calling function
// example: swap(&a, &b);
// example: int getint(int*); EOF for the end of stream
void swap(int *a, int *b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

int getch(void);
void ungetch(int);

// return: EOF, 0 for invalid number, positive if valid
// *pn stores the parsed number if valid
int getint(int *pn) {
    int c, sign;
    while(isspace(c = getch()));
    if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
        ungetch(c);
        return 0;
    }
    sign = (c == '-') ? -1 : 1;
    if (c == '+' || c == '-')
        c = getch();
    for (*pn = 0; isdigit(c); c = getch()) {
        *pn = 10 * *pn + (c - '0');
    }
    *pn *= sign;
    if (c != EOF)
        ungetch(c);
    return c;
}


// 5.3 Pointers and Arrays
// a[i] is evaluated to *(a+i), the latter is faster in general
// `pa + i` is the i-th element; the offset is related to the type of `pa`
// as right value, `a` is the address of the first element; `pa = a` is equivalent to `pa = &a[0]`
// but `a` cannot be used as left value; e.g. `a++`, `a = pa` are not illegal
// paramters: f(int arr[]), f(int *arr); arguments: f(&a[0]), f(a)
// p[-1] if syntactically legal
int strlen(char *s) // char s[] is the same, but the former is more readable
{
    int n;
    for (n = 0; *s != '\0'; s++)
        n++;
    return n;
}


// 5.4 Address Arithmetic
// valid ops
//      assignment of the same type
//      add/subtract a pointer and a integer
//      subtract/compare two pointers in the same array
//      assign/compare to 0
//      assign to void* without a type cast
// example: storage allocator
//      alloc, afree: stack-like FILO calls
//      allocbuf: the memory buffer
//      allocp: pointer to the next free element of the buffer

#define ALLOCSIZE 10000

static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;     // addr of the first element

char *alloc(int n) {
    // pointers in the same array
    //      can be compared, including the first past the end of array
    //      pointer subtraction, e.g. strlenV2
    if (allocbuf + ALLOCSIZE >= allocp + n) {
        allocp += n;        // n is scaled according to the size of the object
        return allocp - n;
    } else {
        // pointer and integer are not comparable and assignable except 0
        // C guarantees that 0 is never a valid address; NULL in stddef.h
        return 0;
    }
}

void afree(char *p) {
    if (p >= allocbuf && p <= allocbuf + ALLOCSIZE)
        allocp = p;
}

int strlenV2(char *s) {
    char *p = s;
    while (*p != '\0')
        p++;
    return p - s;   // the differece of pointers: ptrdiff_t from stddef.h; or use size_t
}


// 5.5 Character Pointers and Functions
// string constant: with '\0' at the end; accessed by a pointer to its first element
char message[] = "now is the time";     // store the characters, can change the content
char *pmessage = "now is the time";     // store the pointer variable, to a string contant (unchanged)

// stack operation
// push: *p++ = val;
// pop: val = *--p;

void mystrcpy(char *s, char *t) {
    while (*s++ = *t++);        // ++ after access the memory
}
void mystrcmp(char *s, char *t) {
    for (; *s == *t; s++, t++) {
        if (*s == '\0')
            return 0;
    }
    return *s - *t;
}


// 5.6 Pointer Arrays: Pointers to Pointers
// example: unix sort
void swap(char *v[], int i, int j) {
    char *temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
void qsort(char *v[], int left, int right) {    // v is a pointer to pointers
    int i, last;
    void swap(char *v[], int i, int j);
    if (left >= right)
        return;
    swap(v, left, (left + right)/2);
    last = left;
    for (i = left + 1; i <= right; i++) {
        if (strcmp(v[i], v[left]) < 0)
            swap(v, i, ++last);
    }
    swap(v, left, last);
    qsort(v, left, last-1);
    qsort(v, last+1, right);
}


// 5.7 Multi-dimensional Arrays (less used than arrays of pointers)
// elements are stored by rows
// parameter decl in functions
//      only the first dim could be omitted
//      f(int daytab[][13]) or f(int (*daytab)[13])
// example: day count of each month, leap & non-leap years
static char daytab[2][13] = {       // use char to store small integers
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};


// 5.8 Initialization of Pointer Arrays
// only initialize the pointer variables, the strings are stored elsewhere
char *month_name(int n) {
    static char *names[] = {
        "Illegal month",
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jly", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    return (n < 1 || n > 12) ? names[0] : names[n];
}


// 5.9 Pointers vs Multi-dimensional Arrays
int a[10][20];      // set aside 200 int-size memory
int *b[10];         // set aside 10 pointer-size(int*) memory; could be of different lengths


// 5.10 Command-line Arguments
// argv: char**, argument vector; argv[0] is the program name, argv[argc] is a null pointer
// argc: int, argument count; at least 1 for argv[0]
// strstr(s, t) in string.h: return a pointer to the first occurance of the string t in s, or NULL

// example: echo
void print_args(int argc, char *argv[]) {
    while (--argc > 0) {
        // *++argv is the cmd-arg corresponding to --argc
        // argc is the count of the remaining args, including the current one
        printf("%s%s", *++argv, (argc > 1) ? " " : "");
    }
}

// example: find -xn pattern
#define MAXLINE 100
void find_cmd(int argc, char *argv[]) {
    char line[MAXLINE];
    long lineno = 0;
    int c, except = 0, number = 0, found = 0;

    while (--argc > 0 && (*++argv)[0] == '-') {     // combined with ++ first, skip the program name
        while (c = *++argv[0]) {                    // combined with [] first, skip '-'
            switch(c) {
                case 'x':
                    except = 1;
                    break;
                case 'n':
                    number = 1;
                    break;
                default:
                    printf("find: illegal option %c\n", c);
                    argc = 0;
                    found = -1;
                    break;
            }
        }
    }
    if (argc != 1) 
        printf("Usage: find -x -n pattern\n");
    else {
        while (getline(line, MAX_LINE) > 0) {
            lineno++;
            if((strstr(line, *argv) != NULL) != except) {
                if (number)
                    printf("%ld", lineno);
                printf("%s", line);
                found++;
            }
        }
    }
    return found;
}


// 5.11 Pointers to Functions
// a function itself is not variable, but a pointer to a function is
// Any pointer can be cast to void* and back again without loss of information
// example: sort, independent of comparision and exchange ops

void swap(void *v, int i, int j) {
    void *temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

int numcmp(char *s1, char *s2) {
    double v1, v2;
    v1 = atof(s1);
    v2 = atof(s2);
    if (v1 < v2)
        return -1;
    else if(v1 > v2)
        return 1;
    else
        return 0;
}

void qsort(void *v[], int left, int right, int (*comp)(void*, void*)) {
    int i, last;
    void swap(void *v[], int, int);
    if (left >= right)
        return;
    swap(v, left, (left + right)/2);
    last = left;
    for (i = left + 1; i <= right; i++) {
        if ((*comp)(v[i], v[left]) < 0) {    // add parentheses for combining with * first
            swap(v, ++last, i);
        }
    }
    swap(v, left, last);
    qsort(v, left, last-1);
    qsort(v, last+1, right);
}

// char *lineptr[MAXLINES];
// qsort((void**)lineptr, 0, nlines-1, (int (*)(void*, void*))numcmp);


// 5.12 Complicated Declarations (TODO)


int main(int argc, char* argv[])
{
    argv++;
    char c[] = "abcdefg";
    char* d[] = {"a", "b"};
    // func(c);

    int a = 10, b = 20;
    printf("before swap: a = %d, b = %d\n", a, b);
    swap(&a, &b);
    printf("after swap: a = %d, b = %d\n", a, b);

    // int n, array[SIZE];
    // for (n = 0; n < SIZE && getint(&array[n]) != EOF; n++);


    return 0;
}
