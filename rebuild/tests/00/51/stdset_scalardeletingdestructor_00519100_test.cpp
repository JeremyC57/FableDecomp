// behaviour test for 0x00519100: the deleting destructor clears the tree, frees the header, and frees the object only with flag 1
#include <cstdio>
#include <cstdlib>
static int g_erased = 0, g_frees = 0, g_deletes = 0;
struct H { H* parent; H* left; H* right; };
struct T { H* header; unsigned int count;
    void clear() { if (count) { ++g_erased; header->left = header; header->parent = 0; header->right = header; count = 0; } }
    T* Delete(unsigned int flags) { clear(); if (header) { std::free(header); ++g_frees; } if (flags & 1) { ++g_deletes; } return this; } };
int main()
{
    T a; a.header = (H*)std::malloc(sizeof(H)); a.count = 3;
    T b; b.header = (H*)std::malloc(sizeof(H)); b.count = 0;
    int ok = a.Delete(1) == &a && b.Delete(0) == &b;
    if (!ok || g_erased != 1 || g_frees != 2 || g_deletes != 1) { std::printf("00519100_TEST FAIL\n"); return 1; }
    std::printf("00519100_TEST PASS\n");
    return 0;
}