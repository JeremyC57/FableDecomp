#include <cstdio>
// Behaviour model of STLport _Rb_tree::_M_insert: create the node (counted copy of the value), link it left of y when y is
// the header, or no hint and (x != 0 or key < key(y)), keep leftmost/rightmost/root current, then count it.
struct Info { unsigned long RefCount; };
struct Node { Node* parent; Node* left; Node* right; long key; void* data; Info* info; };
struct Tree { Node header; unsigned int count; };
static Node pool[8]; static int used;
static Node* insert(Tree& t, Node* x, Node* y, long key, Info* info, Node* w)
{
    Node* z = &pool[used++]; z->key = key; z->data = 0; z->info = info; if (info) ++info->RefCount;
    if (y == &t.header || (w == 0 && (x != 0 || key < y->key))) {
        y->left = z;
        if (y == &t.header) { t.header.parent = z; t.header.right = z; }
        else if (y == t.header.left) t.header.left = z;
    } else {
        y->right = z;
        if (y == t.header.right) t.header.right = z;
    }
    z->parent = y; z->left = 0; z->right = 0;
    ++t.count;
    return z;
}
int main()
{
    Tree t; t.header.parent = 0; t.header.left = &t.header; t.header.right = &t.header; t.count = 0; used = 0;
    Info inf = { 1 };
    Node* a = insert(t, 0, &t.header, 10, &inf, 0);
    if (t.header.parent != a || t.header.left != a || t.header.right != a || inf.RefCount != 2) { std::printf("00553a00_TEST FAIL\n"); return 1; }
    Node* b = insert(t, 0, a, 5, 0, 0);                    // 5 < 10: left of a, new leftmost
    if (a->left != b || t.header.left != b || b->parent != a) { std::printf("00553a00_TEST FAIL\n"); return 1; }
    Node* c = insert(t, 0, a, 20, &inf, 0);                // right of a, new rightmost
    if (a->right != c || t.header.right != c || t.count != 3 || inf.RefCount != 3) { std::printf("00553a00_TEST FAIL\n"); return 1; }
    std::printf("00553a00_TEST PASS\n");
    return 0;
}