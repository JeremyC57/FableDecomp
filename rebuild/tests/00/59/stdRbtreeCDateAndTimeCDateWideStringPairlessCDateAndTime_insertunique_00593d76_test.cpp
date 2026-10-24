// Behaviour test for 0x00593D76 _Rb_tree<CDateAndTime, ...>::insert_unique (model copy over an unbalanced tree)
#include <cstdio>
struct Node { Node* parent; Node* left; Node* right; int key; };
static Node g_pool[16];
static int g_used = 0;
struct Result { Node* first; bool second; };
struct Tree {
    Node* header;
    unsigned int count;
    static Node* decrement(Node* x)
    {
        if (x->left != 0) { Node* y = x->left; while (y->right != 0) y = y->right; return y; }
        Node* y = x->parent;
        while (x == y->left) { x = y; y = y->parent; }
        return y;
    }
    Node* insert(Node* x, Node* y, int v)
    {
        Node* z = &g_pool[g_used++]; z->key = v;
        if (y == header || x != 0 || v < y->key) {
            y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y; z->left = 0; z->right = 0;
        ++count;
        return z;
    }
    Result insert_unique(int v)
    {
        Node* y = header;
        Node* x = header->parent;
        bool comp = true;
        while (x != 0) { y = x; comp = v < x->key; x = comp ? x->left : x->right; }
        Node* j = y;
        Result r;
        if (comp) {
            if (j == header->left) { r.first = insert(y, y, v); r.second = true; return r; }
            j = decrement(j);
        }
        if (j->key < v) { r.first = insert(x, y, v); r.second = true; return r; }
        r.first = j; r.second = false;
        return r;
    }
};
int main()
{
    Node h; h.parent = 0; h.left = &h; h.right = &h; h.key = 0;
    Tree t; t.header = &h; t.count = 0;
    int keys[5] = { 20, 10, 30, 25, 5 };
    for (int i = 0; i < 5; ++i) {
        Result r = t.insert_unique(keys[i]);
        if (!r.second || r.first->key != keys[i]) { std::printf("00593d76_TEST FAIL\n"); return 1; }
    }
    Result dup = t.insert_unique(25);
    if (dup.second || dup.first->key != 25 || t.count != 5) { std::printf("00593d76_TEST FAIL\n"); return 1; }
    dup = t.insert_unique(5);
    if (dup.second || dup.first != h.left) { std::printf("00593d76_TEST FAIL\n"); return 1; }
    if (h.left->key != 5 || h.right->key != 30) { std::printf("00593d76_TEST FAIL\n"); return 1; }
    std::printf("00593d76_TEST PASS\n");
    return 0;
}