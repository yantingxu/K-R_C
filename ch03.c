#include<stdio.h>
#include<ctype.h>
#include<string.h>

// 字符串反转
void reverse(char s[])
{
    int i, j, len = strlen(s);
    int temp;
    // A pair of expressions separated by a comma is evaluated left to right,
    // and the type and value of the result are the type and value of the right operand.
    // not for function arguments, variables in declarations
    for(i = 0, j = len-1; i < j; i++, j--) {
        if(s[i] != s[j]) {
            temp = s[i];
            s[i] = s[j];
            s[j] = temp;
        }
    }
}

// 去掉字符串尾部的空白
// You should verify that this is correct behavior even when the string is empty or contains only white space characters.
int trim(char s[])
{
    int len = strlen(s);
    int i;
    for(i = len-1; i >= 0; i--) {
        if(s[i] != ' ' && s[i] != '\t' && s[i] != '\n')
            break;
    }
    s[i+1] = '\0';
    return i;
}

// 数字转为字符串
void itoa(int n, char s[])
{
    int sign = (n<0) ? -1 : 1;
    int i = 0;
    do {
        s[i++] = n%10 + '0';
    } while((n = n/10) > 0);

    if (sign < 0) {
        s[i++] = '-';
    }
    s[i] = '\0';
    reverse(s);
}

// Exercise 3-3: Expand chars like a-b

// Exercise 3-2: 打印转义字符
void escape(char s[], char t[])
{
    int len = strlen(t);
    int i, j = 0;
    for(i = 0, j = 0; i < len; i++, j++) {
        switch(t[i]) {
            case '\t':
                s[j++] = '\\';
                s[j] = 't';
                break;
            case '\n':
                s[j++] = '\\';
                s[j] = 'n';
                break;
            default:
                s[j] = t[i];
                break;
        }
    }
    s[j] = '\0';
}

// 字符串转为数字
int atoi(char s[])
{
    int i;
    int len = strlen(s);
    for(i = 0; i < len && isspace(s[i]); i++)
        ;

    if(i == len)
        return 0;

    int sign = s[i] == '-' ? -1 : 1;
    if(s[i] == '+' || s[i] == '-') {
        i++;
    }

    int num = 0;
    while(i < len) {
        if(isdigit(s[i])) {
            num = num*10 + s[i]-'0';
            i++;
        } else {
            break;
        }
    }
    return num;
}

// 二分查找（三个测试分支）
int binary_search(int* arr, int n, int v)
{
    int low = 0;
    int high = n-1;
    int mid;
    while (low <= high) {
        mid = (low+high)/2;
        printf("Low: %d, High: %d, Mid: %d\n", low, high, mid);
        if(arr[mid] < v) {
            low = mid + 1;
        } else if(arr[mid] > v) {
            high = mid - 1;
        } else {
            return mid;
        }
    }
    return -1;
}


// Exercise 3-1: 二分查找（两个测试分支）
int binary_search_with_one_test(int* arr, int n, int v)
{
    int low = 0;
    int high = n-1;
    int mid;
    while (low < high) {
        mid = (low+high)/2;
        printf("Low: %d, High: %d, Mid: %d\n", low, high, mid);
        if (arr[mid] < v) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    if (arr[mid] == v) {
        return mid;
    } else {
        return -1;
    }
}

// 统计输入的数字个数
void count_digits()
{
    int i;
    char c;
    int digits[10];
    for (i = 0; i < 10; i++) {
        digits[i] = 0;
    }
    int white = 0, others = 0;

    while ((c = getchar()) != EOF) {
        // printf("Char: %c\n", c);
        // Switch Case: Integer value constant or constant expr
        switch (c) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                digits[c-'0']++;
                break;
            case ' ':
            case '\t':
            case '\n':
                white++;
                break;
            default:
                others++;
                break;
        }
    }

    for (i = 0; i < 10; i++) {
        printf("Digit %d: %d\n", i, digits[i]);
    }
    printf("Whitespace: %d\n", white);
    printf("Non-white: %d\n", others);
}

int main(int argc, char* argv[])
{
    // Notice: dangling else
    char s[15] = "abcdefg   ";
    printf("After Trim: %sEND\n", s);
    trim(s);
    printf("After Trim: %sEND\n", s);

    int i = 12345;
    itoa(i, s);
    printf("ITOA: %sEND\n", s);

    char t[] = "abcdefg\tabcdefg";
    printf("ESCAPE: %sEND\n", t);
    char s0[20];
    escape(s0, t);
    printf("ESCAPE: %sEND\n", s0);

    char s1[] = "12345";
    printf("ATOI: %d\n", atoi(s1));

    // count_digits();

    /*
    const int len = 10;
    int arr[10] = {1, 3, 5, 7, 9, 10, 12, 14, 16, 20};

    int idx;
    printf("Binary Search...\n");
    idx = binary_search(arr, len, 7);
    printf("Index: %d\n", idx);
    idx = binary_search(arr, len, 8);
    printf("Index: %d\n", idx);

    printf("Binary Search (One Test)...\n");
    idx = binary_search_with_one_test(arr, len, 7);
    printf("Index: %d\n", idx);
    idx = binary_search_with_one_test(arr, len, 8);
    printf("Index: %d\n", idx);
    */
}



