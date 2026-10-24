// Behaviour test for 00c18d90: PKCTRRedblackTree::FindNth -- the index-th key in order (0-based, counted through *counter), 0 past the end
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
    void* FindNth(Node* node, long index, long* counter)
    {
        if (node == &Sentinel)
            return 0;
        void* key = FindNth(node->Left, index, counter);
        if (key)
            return key;
        if (*counter == index)
            return node->Key;
        ++*counter;
        return FindNth(node->Right, index, counter);
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
    Tree t;
    unsigned int order[7] = { 40, 20, 60, 10, 30, 50, 70 };
    for (int i = 0; i < 7; ++i) t.Add(order[i]);
    for (long i = 0; i < 7; ++i) {
        long counter = 0;
        void* key = t.FindNth(t.Root, i, &counter);
        if ((unsigned long)key != (unsigned long)(10 * (i + 1))) { std::printf("00c18d90_TEST FAIL\n"); return 1; }
        if (counter != i) { std::printf("00c18d90_TEST FAIL\n"); return 1; }
    }
    long counter = 0;
    if (t.FindNth(t.Root, 7, &counter) != 0 || counter != 7) { std::printf("00c18d90_TEST FAIL\n"); return 1; }   // past the end: 0, every node counted
    Tree empty; counter = 0;
    if (empty.FindNth(empty.Root, 0, &counter) != 0 || counter != 0) { std::printf("00c18d90_TEST FAIL\n"); return 1; }
    std::printf("00c18d90_TEST PASS\n");
    return 0;
}