#include <cstdio>
#include <cstdlib>
struct A {}; struct X { struct node { unsigned char c; unsigned char p[3]; node* parent; node* left; node* right; unsigned char v[0x1c - 16]; }; node* head; unsigned long size;
    X() : head(0) { head = (node*)std::malloc(sizeof(node)); size = 0; head->c = 0; head->parent = 0; head->left = head; head->right = head; } };
int main()
{
    X x; if (!x.head || x.size || x.head->left != x.head || x.head->right != x.head || x.head->parent) { std::printf("0069db20_TEST FAIL\n"); return 1; }
    std::free(x.head);

    std::printf("0069db20_TEST PASS\n");
    return 0;
}