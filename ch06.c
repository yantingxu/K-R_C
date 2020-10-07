/* Chapter 6: Structures
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 6.1 Basics of Structures
// struct decls: define a type
struct point {
    int x;
    int y;
} p1, p2, p3;   // alloc 3 vars at the same time

struct point maxpt = {200, 300};    // with initializer

// member access: dot notation(p1.x), arrow notation(pp1->x)

// nested struct
struct rect {
    struct point pt1;
    struct point pt2;
};


// 6.2 Structures and Functions
// the only ops: copy, assign, take address(&), access members(dot/arrow notation)
// CANNOT be compared
// initialized: with a list of contants, with assignemnt op(only for auto var)
struct point makepoint(int x, int y) {
    struct point temp;
    temp.x = x;
    temp.y = y;
    return temp;        // reutrn by copying the value
}
// parameters: passsed by value; more efficient to pass a pointer
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
void canonrect(struct rect *r) {
    r->pt1.x = min(r->pt1.x, r->pt2.x);
    r->pt1.y = min(r->pt1.y, r->pt2.y);
    r->pt2.x = max(r->pt1.x, r->pt2.x);
    r->pt2.y = max(r->pt1.y, r->pt2.y);
}
// ., ->, func calls(), subscript[] are at the top of precedence hierarchy
// *p->str++, (*p->str)++, *p++->str


// 6.3 Arrays of Structures
struct key {
    char *word;
    int count;
} keytab[] = {
    {"auto", 0},        // the inner brances can be omitted for simple vars
    {"break", 0},
    {"case", 0}
};
// compile-time sizeof: return the size in bytes(stddef.h)
// can be used in #define but not #if, because the preprocessor does not evaluate it
#define NKEYS (sizeof keytab / sizeof keytab[0])
int binsearch(char *word, struct key tab[], int n) {
    int cond;
    int low, high, mid;
    low = 0;
    high = n - 1;
    while (low <= high) {
        mid = (low + high)/2;
        if ((cond = strcmp(word, tab[mid].word)) < 0) {
            high = mid - 1;
        } else if (cond > 0) {
            low = mid + 1;
        } else {
            return mid;
        }
    }
    return -1;
}


// 6.4 Pointers to Structures
struct key *binsearch_p(char *word, struct key *tab, int n) {
    int cond;
    struct key *low = &tab[0];
    struct key *high = &tab[n];         // the pointer past the last element is legal
    struct key *mid;
    while (low < high) {
        mid = low + (high - low)/2;     // subtraction is legal for pointers
        if((cond = strcmp(word, mid->word)) < 0) {
            high = mid;
        } else if (cond > 0) {
            low = mid + 1;
        } else {
            return mid;
        }
    }
    return NULL;
}
// for (p = keytab; p < keytab + NKEYS; p++)
// alignments: the size of a structure is not always the sum of its members, use sizeof instead


// 6.5 Self-referential Structures
// what if the list of words is not known in advance
// use binary tree
struct tnode {
    char *word;
    int count;
    struct tnode *left;     // it is illegal to contain an instance of itself; but pointer is ok
    struct tnode *right;
};

// a word match or a null point found eventually
// addtree return a new tree with node added
struct tnode *talloc(void);
char *mystrdup(char*);

struct tnode *addtree(struct tnode *p, char *w) {
    int cond;
    if (p == NULL) {
        p = talloc();
        p->word = mystrdup(w);    // mystrdup: copy w to some hidden place
        p->count = 1;
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0) {
        p->count++;
    } else if (cond < 0) {
        p->left = addtree(p->left, w);
    } else {
        p->right = addtree(p->right, w);
    }
    return p;
}

// memory allocator: boundary alignment & different types(conversion); use malloc in stdlib.h
struct tnode *talloc(void) {
    return (struct tnode*)malloc(sizeof(struct tnode));
}
char *mystrdup(char *s) {
    char *p;
    p = (char*)malloc(strlen(s) + 1);       // allocate in heap and plus 1 for '\0'
    if (p != NULL)
        strcpy(p, s);
    return p;
}


// 6.6 Table Lookup
// symbol table management: name and replacement text
// main routines: install(s, t), lookup(s)
// implement with hash table and chaining
struct nlist {
    struct nlist *next;
    char *name;
    char *defn;
};
#define HASHSIZE 100
static struct nlist *hashtab[HASHSIZE];     // a table of nlist chains
unsigned hash(char *s) {
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++) {
        hashval = *s + 31 * hashval;
    }
    return hashval % HASHSIZE;
}
struct nlist *lookup(char *s) {
    struct nlist *np;
    // standard idiom to walk through a linked list: for (ptr = head; ptr != NULL; ptr = ptr->next)
    for (np = hashtab[hash(s)]; np != NULL; np = np->next) {
        if (strcmp(np->name, s) == 0)
            return np;
    }
    return NULL;
}
struct nlist *install(char *name, char *defn) {
    struct nlist *np;
    unsigned hashval;
    if ((np = lookup(name)) == NULL) {
        np = (struct nlist*)malloc(sizeof(struct nlist));
        if (np == NULL || (np->name = mystrdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;     // add the new element at the head
    } else {
        free((void*)np->defn);
    }
    if((np->defn = mystrdup(defn)) == NULL)
        return NULL;
    return np;
}


// 6.7 Typedef
// add a new name for some existing type
// just like #define, but it is interpreted by the compiler and hence support more complex things
// two reasons to use: 1. portability, e.g. size_t, ptrdiff_t; 2. better doc
typedef int Length;         // the type name is at the position of a variable name
typedef char *String;       // the type name is conventionally capitalized
typedef int (*PFI)(char*, char*);       // comparators, e.g. strcmp, numcmp

typedef struct nlist *Treeptr;
typedef struct _tnode {
    char *word;
    int count;
    Treeptr left;
    Treeptr right;
} TNode;


// 6.8 Unions
// a single variable holding different types at different times
// can be used within structs and arrays and vice versa
// the compiler keeps track of the size(large enough to hold the largest of all th types) and alignment
// retrived: must be the type most recently stored; dot/arrow notation
// valid ops are the same as struct: assign, copy, take the address, access members
// initialized: must be the value of the type of its first member
union u_tag {
    int ival;
    float fval;
    char *sval;
} u = {10};


// 6.9 Bit-fields
// a set of adjacent bits within a single implementation-defined unit called `word`
// referenced in the same way as other structure members
// almost everything about fields is implementation-dependent
// uname fields (colon and width only) are used for padding; width 0 may be used to force alignment
// give signed/unsigned explicitly for portability
// do not have address, no & allowed
struct {
    unsigned int is_keyword : 1;    // three bits
    unsigned int is_extern : 1;
    unsigned int is_static : 1;
} flags;

int main() {
    printf("hello world\n");
}
