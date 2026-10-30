// Behaviour test for 0x00B988E0 map<long, map<long, CCountedPointer<CEngineSceneMap> > >::operator[] (STLport 4.6).
// The subscript logic (lower_bound walk, insert {k, T()} at the hint when absent, return the value) runs over a model
// tree; the out-of-line callees (inner copy ctor, hinted insert_unique, _M_erase) get simple stand-in bodies.
#include <cstdio>
#include <cstdlib>
#include <new>

struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };

static int g_liveHeaders = 0;
struct Inner {                                  // empty inner map: header node + count + compare
    NodeBase* header; unsigned int count; char cmp;
    Inner() : count(0) { header = (NodeBase*)malloc(0x1c); ++g_liveHeaders; header->color = false; header->parent = 0; header->left = header; header->right = header; }
    Inner(const Inner& x) : count(0) { header = (NodeBase*)malloc(0x1c); ++g_liveHeaders; header->color = false; header->parent = 0; header->left = header; header->right = header; count = x.count; }
    ~Inner() { if (count != 0) { header->left = header; header->parent = 0; header->right = header; count = 0; } if (header) { free(header); --g_liveHeaders; } }
};
struct Value { const long first; Inner second; Value(const long& k, const Inner& v) : first(k), second(v) { } };
struct Node : NodeBase { Value value; };

struct Outer {
    NodeBase* header; unsigned int count;
    Outer() : count(0) { header = (NodeBase*)malloc(sizeof(NodeBase)); header->color = false; header->parent = 0; header->left = header; header->right = header; }
    static long key(NodeBase* x) { return ((Node*)x)->value.first; }
    NodeBase* insert_unique(NodeBase* /*hint*/, const Value& v)   // stand-in: plain BST insert
    {
        Node* z = (Node*)malloc(sizeof(Node));
        new (&z->value) Value(v.first, v.second);
        z->left = z->right = 0; z->color = false;
        NodeBase* y = header; NodeBase* x = header->parent; bool left = true;
        while (x) { y = x; left = v.first < key(x); x = left ? x->left : x->right; }
        z->parent = y;
        if (y == header) { header->parent = z; header->left = z; header->right = z; }
        else if (left) { y->left = z; if (y == header->left) header->left = z; }
        else { y->right = z; if (y == header->right) header->right = z; }
        ++count;
        return z;
    }
    Inner& subscript(const long& k)             // the reconstructed operator[]
    {
        NodeBase* y = header; NodeBase* x = header->parent;
        while (x != 0) { if (!(key(x) < k)) { y = x; x = x->left; } else x = x->right; }
        NodeBase* i = y;
        if (i == header || k < key(i))
            i = insert_unique(i, Value(k, Inner()));
        return ((Node*)i)->value.second;
    }
};

int main()
{
    Outer m;
    Inner& a = m.subscript(5);
    Inner& b = m.subscript(-3);
    Inner& c = m.subscript(12);
    Inner& a2 = m.subscript(5);
    if (&a != &a2 || &a == &b || &b == &c || m.count != 3) { std::printf("00b988e0_TEST FAIL\n"); return 1; }
    if (a.count != 0 || a.header->left != a.header || a.header->parent != 0) { std::printf("00b988e0_TEST FAIL\n"); return 1; }
    if (g_liveHeaders != 3) { std::printf("00b988e0_TEST FAIL\n"); return 1; }   // both temporaries destroyed
    if (Outer::key(m.header->left) != -3 || Outer::key(m.header->right) != 12) { std::printf("00b988e0_TEST FAIL\n"); return 1; }
    std::printf("00b988e0_TEST PASS\n");
    return 0;
}