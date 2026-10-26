// behaviour test for 0x007929A0 std::_Rb_tree::_M_insert [map<CThingCreatureBase*, CTCDivineWrath::PerCreatureInfo>](x, y, v, w): the first node becomes root, leftmost and rightmost; a key below y (or a non-null x hint) links left of y and updates leftmost; w != 0 or a key not below y links right and updates rightmost; the value is copied through its copy constructor once per node; the count grows and the rebalance hook sees each new node with the header's root slot
#include <cstdio>
#include <cstdlib>
#include <new>
typedef unsigned int Key;                          // the instantiation's key (compared with the key type's operator<)
static int gCopies = 0;
static int gRebalanced = 0;
struct Value {                                 // stands for the value type: an out-of-line copy constructor
    int Tag;
    explicit Value(int t) : Tag(t) { }
    Value(const Value& o) : Tag(o.Tag) { ++gCopies; }
};
struct Entry { Key first; Value second; Entry(Key k, int t) : first(k), second(t) { } };
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { Entry value; };
static void Rebalance(NodeBase* x, NodeBase*& root)          // _Rebalance stand-in: records the call, keeps the shape
{
    if (x != 0 && root != 0) ++gRebalanced;
}
struct Tree {
    NodeBase* header; unsigned int count;
    static Key KeyOf(NodeBase* x) { return ((Node*)x)->value.first; }
    NodeBase* Insert(NodeBase* x, NodeBase* y, const Entry& v, NodeBase* w)   // the function under test (STLport 4.6)
    {
        Node* z;
        if (y == header || (w == 0 && (x != 0 || v.first < KeyOf(y)))) {
            z = (Node*)std::malloc(sizeof(Node));
            z->value.first = v.first;
            new (&z->value.second) Value(v.second);
            y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            z = (Node*)std::malloc(sizeof(Node));
            z->value.first = v.first;
            new (&z->value.second) Value(v.second);
            y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y; z->left = 0; z->right = 0;
        Rebalance(z, header->parent);
        ++count;
        return z;
    }
};
int main()
{
    int fail = 0;
    Tree t;
    NodeBase* h = (NodeBase*)std::malloc(sizeof(Node));
    h->color = false; h->parent = 0; h->left = h; h->right = h;
    t.header = h; t.count = 0;
    NodeBase* a = t.Insert(0, h, Entry((Key)20, 2), 0);               // empty tree: y == header
    if (h->parent != a || h->left != a || h->right != a || a->parent != h || t.count != 1) fail |= 1;
    NodeBase* b = t.Insert(0, a, Entry((Key)10, 1), 0);               // below y: left child, new leftmost
    if (a->left != b || h->left != b || h->right != a || b->parent != a || t.count != 2) fail |= 2;
    NodeBase* c = t.Insert(0, a, Entry((Key)30, 3), 0);               // not below y: right child, new rightmost
    if (a->right != c || h->right != c || h->left != b || c->parent != a || t.count != 3) fail |= 4;
    NodeBase* d = t.Insert(c, c, Entry((Key)25, 7), 0);               // x hint: left of y regardless of the compare
    if (c->left != d || h->left != b || d->parent != c || t.count != 4) fail |= 8;
    NodeBase* e = t.Insert(0, b, Entry((Key)5, 9), b);                // w != 0: right of y even for a smaller key
    if (b->right != e || h->right != c || e->parent != b || t.count != 5) fail |= 16;
    if (e->left != 0 || e->right != 0 || ((Node*)d)->value.second.Tag != 7 || Tree::KeyOf(d) != (Key)25) fail |= 32;
    if (gCopies != 5 || gRebalanced != 5) fail |= 64;
    if (fail) { std::printf("007929a0_TEST FAIL %d\n", fail); return 1; }
    std::printf("007929a0_TEST PASS\n");
    return 0;
}