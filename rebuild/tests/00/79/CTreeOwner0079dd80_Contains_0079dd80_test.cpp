#include <cstdio>
struct CRetailTree_longkey {
    struct node { unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; long key; };
    struct iterator { node* current; iterator(node* n) : current(n) { } };
    node* head; unsigned long size;
    iterator find_0079f210(long key) {
        node* result = head; node* n = head->parent;
        while (n) { if (!(n->key < key)) { result = n; n = n->left; } else n = n->right; }
        if (result == head || key < result->key) result = head;
        return iterator(result);
    }
};
struct CTreeOwner_0079dd80 { unsigned char _pad[0x48]; CRetailTree_longkey m_tree; long Contains(long key) { CRetailTree_longkey::iterator it = m_tree.find_0079f210(key); return it.current != m_tree.head; } };
int main()
{
    typedef CRetailTree_longkey::node node;
    node h = { 0 }, a = { 0 }, b = { 0 }, c = { 0 };
    a.key = 10; b.key = 5; c.key = 20; a.left = &b; a.right = &c; b.parent = &a; c.parent = &a; a.parent = &h; h.parent = &a; h.left = &b; h.right = &c;
    CTreeOwner_0079dd80 o; o.m_tree.head = &h; o.m_tree.size = 3;
    if (!o.Contains(10) || !o.Contains(5) || !o.Contains(20) || o.Contains(7) || o.Contains(0) || o.Contains(30) || (char*)&o.m_tree - (char*)&o != 0x48) { std::printf("0079dd80_TEST FAIL\n"); return 1; }
    std::printf("0079dd80_TEST PASS\n"); return 0;
}