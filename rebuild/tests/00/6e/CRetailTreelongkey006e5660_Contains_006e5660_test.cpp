#include <cstdio>
struct CRetailTree_longkey_006e5660 {
    struct node { unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; long key; };
    struct iterator { node* current; iterator(node* n) : current(n) { } };
    node* head; unsigned long size;
    iterator find(long key) {   // lower-bound walk as in the retail find
        node* result = head; node* n = head->parent;
        while (n) { if (!(n->key < key)) { result = n; n = n->left; } else n = n->right; }
        if (result == head || key < result->key) result = head;
        return iterator(result);
    }
    long Contains(long key) { iterator it = find(key); return it.current != head; }
};
int main()
{
    typedef CRetailTree_longkey_006e5660::node node;
    node h = { 0 }, a = { 0 }, b = { 0 }, c = { 0 };
    a.key = 10; b.key = 5; c.key = 20; a.left = &b; a.right = &c; b.parent = &a; c.parent = &a; a.parent = &h; h.parent = &a; h.left = &b; h.right = &c;
    CRetailTree_longkey_006e5660 t; t.head = &h; t.size = 3;
    if (!t.Contains(10) || !t.Contains(5) || !t.Contains(20) || t.Contains(7) || t.Contains(0) || t.Contains(30)) { std::printf("006e5660_TEST FAIL\n"); return 1; }
    std::printf("006e5660_TEST PASS\n"); return 0;
}