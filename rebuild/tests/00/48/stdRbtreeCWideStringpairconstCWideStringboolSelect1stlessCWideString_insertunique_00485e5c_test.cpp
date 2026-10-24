// Behaviour test for 0x00485E5C _Rb_tree<CWideString, pair<const CWideString, bool> >::insert_unique (model on an unbalanced BST).
#include <cstdio>
#include <cstdlib>

struct Node { Node* parent; Node* left; Node* right; long key; bool value; };
struct Result { Node* it; bool inserted; };

struct Tree {
    Node header;
    int count;
    Tree() : count(0) { header.parent = 0; header.left = &header; header.right = &header; }
    static Node* Decrement(Node* x)
    {
        if (x->left) { x = x->left; while (x->right) x = x->right; return x; }
        Node* y = x->parent;
        while (x == y->left) { x = y; y = y->parent; }
        return y;
    }
    Node* Insert(Node* x, Node* y, long key, bool value)
    {
        Node* z = (Node*)std::malloc(sizeof(Node));
        z->key = key; z->value = value; z->left = z->right = 0; z->parent = y;
        if (y == &header || x != 0 || key < y->key) {
            y->left = z;
            if (y == &header) { header.parent = z; header.right = z; }
            else if (y == header.left) header.left = z;
        } else {
            y->right = z;
            if (y == header.right) header.right = z;
        }
        ++count;
        return z;
    }
    Result InsertUnique(long key, bool value)
    {
        Node* y = &header;
        Node* x = header.parent;
        bool comp = true;
        while (x != 0) { y = x; comp = key < x->key; x = comp ? x->left : x->right; }
        Node* j = y;
        if (comp) {
            if (j == header.left) { Result r = { Insert(y, y, key, value), true }; return r; }
            j = Decrement(j);
        }
        if (j->key < key) { Result r = { Insert(x, y, key, value), true }; return r; }
        Result r = { j, false };
        return r;
    }
    void Free(Node* n) { if (n) { Free(n->left); Free(n->right); std::free(n); } }
};

int main()
{
    Tree t;
    Result a = t.InsertUnique(5, true);
    Result b = t.InsertUnique(2, false);
    Result c = t.InsertUnique(8, true);
    Result d = t.InsertUnique(5, false);      // duplicate: not inserted, returns the existing node
    Result e = t.InsertUnique(3, true);
    int ok = a.inserted && b.inserted && c.inserted && !d.inserted && d.it == a.it && d.it->value
          && e.inserted && t.count == 4 && t.header.left == b.it && t.header.right == c.it;
    t.Free(t.header.parent);
    if (!ok) { std::printf("00485e5c_TEST FAIL\n"); return 1; }
    std::printf("00485e5c_TEST PASS\n");
    return 0;
}