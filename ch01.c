/* Chapter 1: A Tutorial Introduction
 */
#include<stdio.h>

void hello_world()
{
    // the standard library ensures that an input text stream appears as a sequence of lines
    printf("hello world!\n");
    printf("%6.2f\n", -3.33333333);     // 长度包括小数点也包括负号
}

// instead of magic number, use macro to make it more readable
#define LOW 0
#define HIGH 300
#define STEP 20

void degree_convert()
{
    float c, f;
    f = LOW;
    while (f < HIGH) {
        c = (5.0/9.0)*(f-32);
        printf("%6.1f => %3.1f\n", f, c);
        f += STEP;
    }
}

void for_degree_convert()
{
    float f;
    for (f = LOW; f < HIGH; f += STEP) {
        printf("%6.1f => %3.1f\n", f, (5.0/9.0)*(f-32));
    }
}

void file_copy()
{
    int c;      // must be int to cover EOF
    while ((c = getchar()) != EOF) {
        putchar(c);
    }
    // putchar(c);
}

void char_counter()
{
    // 'A' is to be preferred over 65: its meaning is obvious, and it is independent of a particular character set. 
    // '\n' != "\n"
    long c;     // for bigger number, use double
    for (c = 0; getchar() != EOF; c++);
    printf("%ld characters counted!\n", c);   // ld for long integer
}

void line_counter()
{
	int c;
	long counter = 0;
	while ((c = getchar()) != EOF) {
		if (c == '\n') {
			counter++;
		}
	}
	printf("%ld lines counted!\n", counter);
}

#define IN 0
#define OUT 1
void word_counter()
{
    int c, nl, nw, nc, state;
    nl = nw = nc = 0;
    state = OUT;
    while ((c = getchar()) != EOF) {
        nc++;
        if (c == '\n') {
            nl++;
        }
        if (c == ' ' || c == '\n' || c == '\t') {
            state = OUT;
        } else {
            if (state == OUT) {
                state = IN;
                nw++;
            }
        }
    }
}

void digit_counter()
{
    int c, i, nwhite, nother;
    int digits[10];

    nwhite = nother = 0;
    for (i = 0; i < 10; i++) {
        digits[i] = 0;
    }

    while ((c = getchar()) != EOF) {
        if (c >= '0' && c <= '9') {
            digits[c-'0']++;
        } else if (c == ' ' || c == '\t' || c == '\n') {
            nwhite++;
        } else {
            nother++;
        }
    }
    printf("Digits = ");
    for (i = 0; i < 10; i++) {
        printf(" %d", digits[i]);
    }
    printf("; White = %d; Other = %d\n", nwhite, nother);
}

int power(int base, int n)
{
    // in old-style function, the params and return value are assumed to be int
    // all args are passed by value; call by ref could be made by passsing address (including array name)
    int i;
    int result = 1;
    for (i = 0; i < n; i++) {
        result *= base;
    }
    return result;
}

#define MAXLEN 1000

// But for compatibility with older C programs the standard takes an empty list as an old-style declaration, and turns off all argument list checking; the word void must be used for an explicitly empty list.
int mygetline(char s[], int lim);
void copy(char from[], char to[]);

// An external variable must be defined, exactly once, outside of any function; this sets aside storage for it. 
// The variable must also be declared in each function that wants to access it; this states the type of the variable. The declaration may be an explicit extern statement or may be implicit from context.
// The usual practice is to collect extern declarations of variables and functions in a separate file, historically called a header.

void longest_line()
{
    int len;
    char line[MAXLEN];

    int max = 0;
    char longest[MAXLEN];

    while (len = mygetline(line, MAXLEN)) {
        if (len > max) {
            max = len;
            copy(line, longest);
        }
    }

    if (max > 0) {
        printf("Longest: %s", longest);
    }
}

// Every text line has at least one character; even a line containing only a newline has length 1.
int mygetline(char s[], int lim)
{
    int c, i;
    for (i = 0; i < lim-1 && (c = getchar()) != EOF && c != '\n'; i++) {
        s[i] = c;
    }
    if (c == '\n') {
        s[i] = c;
        i++;
    }
    s[i] = '\0';
    return i;
}

void copy(char from[], char to[])
{
    int i = 0;
    while ((to[i] = from[i]) != '\0')
        i++;
}

int main()
{
    hello_world();
    // degree_convert();
    // for_degree_convert();
    // file_copy();
    // char_counter();
	// line_counter();
    // digit_counter();
    // printf("%d\n", power(2, 3));
    longest_line();
    return 0;
}

