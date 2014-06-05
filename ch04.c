// 4.3 External variables
// External variables are defined outside of any function, and are thus potentionally available to many functions.
// Functions themselves are always external, because C does not allow functions to be defined inside other functions
// reverse Polish notation
// push(pop() - pop());   /* WRONG */

// 4.4 Scope Rules
// local vs external variables: lifetime
// if an external variable is to be referred to before it is defined, or if it is defined in a different source file from the one where it is being used, then an extern declaration is mandatory.
// Array sizes must be specified with the definition, but are optional with an extern declaration. 

// 4.5 Header File
// Up to some moderate program size, it is probably best to have one header file that contains everything that is to be shared between any two parts of the program.

// 4.6 Static Variables
// If a function/variable is declared static, however, its name is invisible outside of the file in which it is declared. 
// The static declaration can also be applied to internal variables.

// 4.7 Register Variables
// The register declaration can only be applied to automatic variables and to the formal parameters of a function
// Excess register declarations are harmless, however, since the word register is ignored for excess or disallowed declarations. And it is not possible to take the address of a register variable (a topic covered in Chapter 5), regardless of whether the variable is actually placed in a register. 

// 4.8 Block Structure
// scope inside {}

// 4.9 Init
// In the absence of explicit initialization, external and static variables are guaranteed to be initialized to zero; automatic and register variables have undefined (i.e., garbage) initial values. 
// For external and static variables, the initializer must be a constant expression; the initialization is done once, conceptionally before the program begins execution. For automatic and register variables, the initializer is not restricted to being a constant: it may be any expression involving previously defined values, even function calls.
// char pattern = "ould";
// If there are fewer initializers for an array than the specified size, the others will be zero for external, static and automatic variables. It is an error to have too many initializers. There is no way to specify repetition of an initializer, nor to initialize an element in the middle of an array without supplying all the preceding values as well. 

// 4.10 Recursion
// qsort

// 4.11 Preprocessor
// There are often several #include lines at the beginning of a source file, to include common #define statements and extern declarations, or to access the function prototype declarations for library functions from headers like <stdio.h>
// The scope of a name defined with #define is from its point of definition to the end of the source file being compiled. A definition may use previous definitions.
// #define  max(A, B)  ((A) > (B) ? (A) : (B))
// this is bad if they involve side effects like increment operators or input and output
// Names may be undefined with #undef, usually to ensure that a routine is really a function, not a macro
// stdio.h, ctype.h: defined as macros to avoid the run-time overhead of a function
// If, however, a parameter name is preceded by a # in the replacement text, the combination will be expanded into a quoted string with the parameter replaced by the actual argument, e.g. #define  dprint(expr)   printf(#expr " = %g\n", expr)
// The preprocessor operator ## provides a way to concatenate actual arguments during macro expansion. #define  paste(front, back)  front ## back
/*
The #if line evaluates a constant integer expression (which may not include sizeof, casts, or enum constants). If the expression is non-zero, subsequent lines until an #endif or #elif or #else are included. (The preprocessor statement #elif is like else-if.) The expression defined(name) in a #if is 1 if the name has been defined, and 0 otherwise. 

#if !defined(HDR) OR #ifndef HDR
#define HDR
#endif

#if SYSTEM == SYSV
   #define HDR "sysv.h"
#elif SYSTEM == BSD
   #define HDR "bsd.h"
#elif SYSTEM == MSDOS
   #define HDR "msdos.h"
#else
   #define HDR "default.h"
#endif
#include HDR
*/

#include<stdio.h>


// 4.1 - 4.2
// If the return type is omitted, int is assumed.
// The functions can occur in any order in the source file, and the source program can be split into multiple files, so long as no function is split. 
// In any case, if a function fails to return a value, its ``value'' is certain to be garbage. 
// if only main.c is modified, just run cc main.c getline.o strindex.o
/*
double sum, atof(char []);
Function declaration could appear in the body of another function, e.g. main
But if (as is more likely) atof were compiled separately, the mismatch would not be detected, atof would return a double that main would treat as an int, and meaningless answers would result. 
The reason a mismatch can happen is that if there is no function prototype, a function is implicitly declared by its first appearance in an expression
Furthermore, if a function declaration does not include arguments, that too is taken to mean that nothing is to be assumed about the arguments of atof; all parameter checking is turned off. Use void instead
*/

/* strindex:  return index of t in s, -1 if none */
int strindex(char s[], char t[])
{
    int i, j, k;
    for (i = 0; s[i] != '\0'; i++) {
        for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++);
        if (t[k] == '\0') {
            return i;
        }
    }
    return -1;
}

int getline(char s[], int lim)
{
    int i, c;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; i++) {
        s[i] = c;
    }
    if (c == '\n') {
        s[i++] = c;
    }
    s[i] = '\0';
    return i;
}

/* atof:  convert string s to double */
double atof(char s[])
{
    int i, sign;
    for (i = 0; isspace(s[i]); i++);
    if (s[i] == '-')
        sign = -1;
    if (s[i] == '+' || s[i] == '-')
        i++;

    double val = 0.0;
    double power = 1.0;
    while (isdigit(s[i])) {
        val = 10.0*val + s[i] - '0';
        i++;
    }
    if (s[i] == '.') {
        i++;
        while (isdigit(s[i])) {
            val = 10.0*val + s[i] - '0';
            power *= 10.0;
        }
    }
    return sign*val/power;
}

int main()
{
    char tmp[4] = "abc";
    printf("Tmp: %s\n", tmp);
}




