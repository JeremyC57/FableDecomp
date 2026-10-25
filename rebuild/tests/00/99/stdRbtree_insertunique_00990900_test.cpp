// Behaviour test for 0x00990900 _Rb_tree<CPendingObjectSortKey, ..>::insert_unique(v) (STLport 4.6 unique insert).
#include <cstdio>
#include <cstdlib>

struct CThreadedFile { unsigned int PhysicalSortKey; };
struct CFileObject { unsigned int FilePos; CThreadedFile* PFile; };
struct Key {
    CFileObject* Object;
    bool operator<(const Key& o) const
    {
        unsigned int a = Object->PFile->PhysicalSortKey, b = o.Object->PFile->PhysicalSortKey;
        if (a < b) return true;
        if (a > b) return false;
        if (Object->FilePos < o.Object->FilePos) return true;
        if (Object->FilePos > o.Object->FilePos) return false;
        return Object < o.Object;
    }
};
struct Node { bool color; Node* parent; Node* left; Node* right; Key key; CFileObject* value; };

struct Tree {
    Node* header; unsigned int count;
    Tree() { header = new Node(); header->parent = 0; header->left = header; header->right = header; count = 0; }
    Node* Insert(Node* x, Node* y, const Key& k)
    {
        Node* z = new Node(); z->key = k; z->value = k.Object;
        if (y == header || x != 0 || k < y->key) {
            y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y; z->left = 0; z->right = 0; ++count;
        return z;
    }
    static Node* Decrement(Node* n, Node* header)
    {
        if (n == header) return n->right;
        if (n->left) { Node* y = n->left; while (y->right) y = y->right; return y; }
        Node* y = n->parent; while (n == y->left) { n = y; y = y->parent; } return y;
    }
    // the retail body: descend, step back once unless at begin(), insert when new
    Node* InsertUnique(const Key& v, bool& inserted)
    {
        Node* y = header; Node* x = header->parent; bool comp = true;
        while (x != 0) { y = x; comp = v < x->key; x = comp ? x->left : x->right; }
        Node* j = y;
        if (comp) {
            if (j == header->left) { inserted = true; return Insert(y, y, v); }
            j = Decrement(j, header);
        }
        if (j->key < v) { inserted = true; return Insert(x, y, v); }
        inserted = false; return j;
    }
};

int main()
{
    CThreadedFile f1 = { 2 }, f2 = { 1 };
    CFileObject o[4];
    o[0].FilePos = 10; o[0].PFile = &f1;
    o[1].FilePos = 5;  o[1].PFile = &f1;
    o[2].FilePos = 50; o[2].PFile = &f2;
    o[3].FilePos = 7;  o[3].PFile = &f1;
    Tree t; bool ins;
    for (int i = 0; i < 4; ++i) {
        Key k; k.Object = &o[i];
        Node* n = t.InsertUnique(k, ins);
        if (!ins || n->key.Object != &o[i]) { std::printf("00990900_TEST FAIL\n"); return 1; }
    }
    Key dup; dup.Object = &o[3];
    Node* n = t.InsertUnique(dup, ins);
    if (ins || n->key.Object != &o[3] || t.count != 4) { std::printf("00990900_TEST FAIL\n"); return 1; }
    // in-order: o[2] (sort key 1), then f1 by FilePos: o[1] (5), o[3] (7), o[0] (10)
    CFileObject* want[4] = { &o[2], &o[1], &o[3], &o[0] };
    if (t.header->left->key.Object != want[0] || t.header->right->key.Object != want[3]) { std::printf("00990900_TEST FAIL\n"); return 1; }
    Node* cur = t.header->right;
    for (int i = 3; i >= 0; --i) {
        if (cur->key.Object != want[i]) { std::printf("00990900_TEST FAIL\n"); return 1; }
        cur = Tree::Decrement(cur, t.header);
    }
    std::printf("00990900_TEST PASS\n");
    return 0;
}