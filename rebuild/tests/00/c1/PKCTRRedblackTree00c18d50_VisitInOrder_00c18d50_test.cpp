// Behaviour test for 00c18d50: PKCTRRedblackTree::VisitInOrder -- in-order walk handing each node's key slot to the visitor (recursion left, loop right)
#include <cstdio>

static unsigned int K(void* p) { return (unsigned int)(unsigned long)p; }
static unsigned int K(unsigned int k) { return k; }

struct Node {
    Node* Left;
    Node* Right;
    Node* Parent;
    long  Colour;
    void* Key;
    void* Value;
};
enum { BLACK = 0, RED = 1 };

struct Tree {
    Node  Sentinel;
    Node* Root;
    Node* LastFound;
    Node  pool[64];
    int   used;
    Tree() : Root(&Sentinel), LastFound(0), used(0)
    {
        Sentinel.Left = Sentinel.Right = &Sentinel;
        Sentinel.Parent = 0;
        Sentinel.Colour = BLACK;
    }
    // plain (unbalanced) BST insert used to build fixtures
    Node* Add(unsigned int key)
    {
        Node* n = &pool[used++];
        n->Left = n->Right = &Sentinel; n->Colour = RED; n->Key = (void*)(unsigned long)key; n->Value = (void*)(pool + key % 64);
        Node* p = 0; Node* c = Root;
        while (c != &Sentinel) { p = c; c = (K(n->Key) < K(c->Key)) ? c->Left : c->Right; }
        n->Parent = p;
        if (!p) Root = n; else if (K(n->Key) < K(p->Key)) p->Left = n; else p->Right = n;
        return n;
    }
    void RotateLeft(Node* x)
    {
        Node* y = x->Right;
        x->Right = y->Left;
        if (y->Left != &Sentinel) y->Left->Parent = x;
        if (y != &Sentinel) y->Parent = x->Parent;
        if (x->Parent) {
            if (x == x->Parent->Left) x->Parent->Left = y;
            else x->Parent->Right = y;
        } else {
            Root = y;
        }
        y->Left = x;
        if (x != &Sentinel) x->Parent = y;
    }
    void RotateRight(Node* x)
    {
        Node* y = x->Left;
        x->Left = y->Right;
        if (y->Right != &Sentinel) y->Right->Parent = x;
        if (y != &Sentinel) y->Parent = x->Parent;
        if (x->Parent) {
            if (x == x->Parent->Right) x->Parent->Right = y;
            else x->Parent->Left = y;
        } else {
            Root = y;
        }
        y->Right = x;
        if (x != &Sentinel) x->Parent = y;
    }
    struct Visitor { virtual void Visit(void** key) = 0; };
    void VisitInOrder(Node* node, Visitor* visitor)
    {
        while (node != &Sentinel) {
            VisitInOrder(node->Left, visitor);
            visitor->Visit(&node->Key);
            node = node->Right;
        }
    }

};

// in-order key dump + parent-link check
static int Dump(Tree& t, Node* n, unsigned int* out, int k)
{
    if (n == &t.Sentinel) return k;
    if (n->Left != &t.Sentinel && n->Left->Parent != n) return -1000;
    if (n->Right != &t.Sentinel && n->Right->Parent != n) return -1000;
    k = Dump(t, n->Left, out, k);
    if (k < 0) return k;
    out[k++] = K(n->Key);
    return Dump(t, n->Right, out, k);
}
static bool Sorted(Tree& t, int expect)
{
    unsigned int keys[64];
    int n = Dump(t, t.Root, keys, 0);
    if (n != expect) return false;
    for (int i = 1; i < n; ++i) if (keys[i - 1] >= keys[i]) return false;
    return t.Root == &t.Sentinel || t.Root->Parent == 0;
}

int main()
{
    struct Recorder : Tree::Visitor {
        unsigned int keys[16]; int n; void** last;
        void Visit(void** key) { last = key; keys[n++] = (unsigned int)(unsigned long)*key; }
    };
    Tree t;
    unsigned int order[9] = { 50, 30, 70, 20, 40, 60, 80, 35, 65 };
    for (int i = 0; i < 9; ++i) t.Add(order[i]);
    Recorder r; r.n = 0; r.last = 0;
    t.VisitInOrder(t.Root, &r);
    if (r.n != 9) { std::printf("00c18d50_TEST FAIL\n"); return 1; }
    for (int i = 1; i < 9; ++i) if (r.keys[i - 1] >= r.keys[i]) { std::printf("00c18d50_TEST FAIL\n"); return 1; }
    if (r.keys[0] != 20 || r.keys[8] != 80) { std::printf("00c18d50_TEST FAIL\n"); return 1; }
    Recorder e; e.n = 0; e.last = 0;
    Tree empty;
    empty.VisitInOrder(empty.Root, &e);
    if (e.n != 0) { std::printf("00c18d50_TEST FAIL\n"); return 1; }
    Recorder s; s.n = 0; s.last = 0;
    t.VisitInOrder(t.Root->Left, &s);      // a subtree only
    if (s.n != 4 || s.keys[0] != 20 || s.keys[3] != 40) { std::printf("00c18d50_TEST FAIL\n"); return 1; }
    std::printf("00c18d50_TEST PASS\n");
    return 0;
}