// 5.1 Pointer and Address
// The & operator only applies to objects in memory: variables and array elements. It cannot be applied to expressions, constants, or register variables.
// ip = &z[0];       /* ip now points to z[0] */
// every pointer points to a specific data type. (There is one exception: a ``pointer to void'' is used to hold any type of pointer but cannot be dereferenced itself)
// ++*ip and (*ip)++, unary operators like * and ++ associate right to left

// 5.2
// Since C passes arguments to functions by value, there is no direct way for the called function to alter a variable in the calling function;  swap(&a, &b);

#include<stdio.h>

void func(char s[])
{
    s = s + 1;
    printf("%c", *s);
}
int main(int argc, char* argv[])
{
    argv++;
    char c[] = "abcdefg";
    char* d[] = {"a", "b"};
    // func(c);
    return 0;
}
