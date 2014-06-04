/*
 * Chapter 2: Types, Operators and Expressions
 */

#include<stdio.h>
#include<ctype.h>

void var_name()
{
    // letters + digits + underscore
    // case sensitive
    // lower case for varibales and uppder case for constants
    // first 31 letters, first 6 in single case
    // short for local vars, long for external vars
    return;
}

void data_types()
{
    // char: one character; int: natural size; short 16bits; long >= 32bits
    // signed/unsigned: whether char is or not is machine-dependent; so is float, double, long double
    // limits.h, float.h
    return;
}

void constants()
{
    // suffix: L, UL, F, L; hex, oct...
    // char constant: appears a character, actually an integer, such as 'a'
    // escape char: '\ooo' '\xhh' '\0'
    // constant expr: evaluated at compile time; #define MAXLEN 100; char line[MAXLEN+1];
    // string constant: with double quotes around, including empty ""; could be concat at compile time, such as "hello, " "world"
    // strlen(s): excluding the tailing '\0'; in string.h
    // warning: 'x' is not the same as "x"
    // enum constant: enum boolean { NO, YES };
    // Names in different enumerations must be distinct. Values need not be distinct in the same enumeration.
    enum XEnum {A = 1, B, C};
    enum YEnum {D = 100};
    printf("%d\n", D);
    enum YEnum sample = B;      // no checking here
    printf("%d\n", sample);
    return;
}

void declaration()
{
    // If the variable in question is not automatic, the initialization is done once only, conceptionally before the program starts executing, and the initializer must be a constant expression. An explicitly initialized automatic variable is initialized each time the function or block it is in is entered; the initializer may be any expression. External and static variables are initialized to zero by default. Automatic variables for which is no explicit initializer have undefined (i.e., garbage) values.
    // const char msg[] = "warning"; not make the variable a constant one but a unchanged var
    return;
}

int is_leap_year(int year)
{
    // negaitve or float for % is machine-dependent
    // By definition, the numeric value of a relational or logical expression is 1 if the relation is true, and 0 if the relation is false. 
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) {
        return 1;
    } else {
        return 0;
    }
}

int mystrlen(char s[])
{
    int i = 0;
    while (s[i] != '\0') {
        i++;
    }
    return i;
}

int myatoi(char s[])
{
    int result = 0;
    int i;
    for (i = 0; s[i] >= '0' && s[i] <= '9'; i++)
        result = 10*result + s[i] - '0';
    return result;
}

void type_convertion()
{
    // in ctype.h, lower => tolower, 0-9 => isdigit(c)
    // char to int, signed/unsigned expansion is not determined; it is better to claim char is signed or unsigned
    // visible character is always positive
    // arth conversion: if no unsigned operand, lower operand to wider operand; for singed, For example, suppose that int is 16 bits and long is 32 bits. Then -1L < 1U, because 1U, which is an unsigned int, is promoted to a signed long. But -1L > 1UL because -1L is promoted to unsigned long and thus appears to be a large positive number.
    // Longer integers are converted to shorter ones or to chars by dropping the excess high-order bits. Therefore, no loss information where sign extention or not.
    // In the absence of a function prototype, char and short become int, and float becomes double.
    return;
}

int lower(int c)
{
    if (c >= 'A' && c <= 'Z') {
        c = c + 'a' - 'A';
    }
    return c;
}

void incremental_operator()
{
    // Ilegal: (i+j)++;
    // mysqueeze
    // mystrcat
    return;
}

void mysqueeze(char s[], char c)
{
    int i, j;
    for (i = j = 0; s[i] != '\0'; i++) {
        if (s[i] != c) {
            s[j++] = s[i];
        }
    }
    s[j] = '\0';
}

void mystrcat(char s[], char t[])
{
    int i, j;
    for (i = 0; s[i] != '\0'; i++);
    for (j = 0; t[j] != '\0'; j++) {
        s[i++] = t[j];
    }
    s[i] = '\0';
}

void bitwise_operator()
{
    // applied to any integral operand, signed or unsigned
    // &: mask off; |: set on; ^: reverse
    // right shift: for unsigned integral, always filled with zeros; for signed integral, sign extention or not is machine dependent
    // Note that x & ~077 is independent of word length
    // getbits
    return;
}

unsigned getbits(unsigned x, int p, int n)
{
    return (x >> (p-n+1)) & ~(~0 << n);
}

void assign_operator()
{
    // exp1 op= exp2 is the same as exp1 = exp1 op exp2, except that exp1 is evaluated only once
    // bitcount
    return;
}

int bitcount(unsigned x)
{
    int count = 0;
    while (x > 0) {
        if (x & 01) {
            count++;
        }
        x >>= 1;
    }
    return count;
}

void conditional_operator()
{
    // z = (a > b) ? a : b;    /* z = max(a, b) */
    // If expr2 and expr3 are of different types, the type of the result is determined by the conversion rules
    // printf("You have %d items%s.\n", n, n==1 ? "" : "s");
    return;
}

void precedence()
{
    // if ((a & mask) == 0)
    // x = f() + g(); no order ensured
    // Similarly, the order in which function arguments are evaluated is not specified: printf("%d %d\n", ++n, power(2, n));   /* WRONG */
    // side effects: a[i] = i++;
    return;
}

int main()
{
    // constants();
    return 0;
}
