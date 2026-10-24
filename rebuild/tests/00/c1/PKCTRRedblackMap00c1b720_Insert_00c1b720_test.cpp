// Behaviour test for 00c1b720: PKCTRRedblackMap::Insert -- red-black insert by unsigned key: 2 duplicate, 1 allocation failure, 0 inserted (fixup run, cache cleared)
#include <cstdio>

static unsigned int K(void* p) { return (unsigned int)(unsigned long)p; }
static unsigned int K(unsigned int k) { return k; }

struct Node {
    Node* Left;
    Node* Right;
    Node* Parent;
    long  Colour;
    unsigned int Key;
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
        n->Left = n->Right = &Sentinel; n->Colour = RED; n->Key = (unsigned int)(unsigned long)key; n->Value = (void*)(pool + key % 64);
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
    void InsertFixup(Node* x)
    {
        while (x != Root && x->Parent->Colour == RED) {
            if (x->Parent == x->Parent->Parent->Left) {
                Node* y = x->Parent->Parent->Right;
                if (y->Colour == RED) {
                    x->Parent->Colour = BLACK; y->Colour = BLACK; x->Parent->Parent->Colour = RED;
                    x = x->Parent->Parent;
                } else {
                    if (x == x->Parent->Right) { x = x->Parent; RotateLeft(x); }
                    x->Parent->Colour = BLACK; x->Parent->Parent->Colour = RED;
                    RotateRight(x->Parent->Parent);
                }
            } else {
                Node* y = x->Parent->Parent->Left;
                if (y->Colour == RED) {
                    x->Parent->Colour = BLACK; y->Colour = BLACK; x->Parent->Parent->Colour = RED;
                    x = x->Parent->Parent;
                } else {
                    if (x == x->Parent->Left) { x = x->Parent; RotateRight(x); }
                    x->Parent->Colour = BLACK; x->Parent->Parent->Colour = RED;
                    RotateLeft(x->Parent->Parent);
                }
            }
        }
        Root->Colour = BLACK;
    }
    long allocFail;
    long Insert(const unsigned int* key, void* const* value)
    {
        Node* parent = 0;
        Node* current = Root;
        while (current != &Sentinel) {
            if (*key == current->Key)
                return 2;
            parent = current;
            if (*key < current->Key)
                current = current->Left;
            else
                current = current->Right;
        }
        Node* x = allocFail ? 0 : &pool[used++];
        if (x == 0)
            return 1;
        x->Parent = parent;
        x->Left = &Sentinel;
        x->Right = &Sentinel;
        x->Colour = RED;
        x->Key = *key;
        x->Value = *value;
        if (parent) {
            if (*key < parent->Key)
                parent->Left = x;
            else
                parent->Right = x;
        } else {
            Root = x;
        }
        InsertFixup(x);
        LastFound = 0;
        return 0;
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
    t.allocFail = 0;
    int marker[40];
    for (unsigned int i = 1; i <= 31; ++i) {          // ascending inserts: the fixup must keep rebalancing
        void* v = &marker[i];
        t.LastFound = t.Root;
        if (t.Insert(&i, &v) != 0 || t.LastFound != 0) { std::printf("00c1b720_TEST FAIL\n"); return 1; }
    }
    if (!Sorted(t, 31) || t.Root->Colour != BLACK) { std::printf("00c1b720_TEST FAIL\n"); return 1; }
    if (t.Root->Key != 8 && t.Root->Key != 16) { std::printf("00c1b720_TEST FAIL\n"); return 1; }  // balanced: a middle key at the root
    unsigned int dup = 17;
    void* v = 0;
    if (t.Insert(&dup, &v) != 2 || !Sorted(t, 31)) { std::printf("00c1b720_TEST FAIL\n"); return 1; }
    t.allocFail = 1;
    unsigned int fresh = 99;
    if (t.Insert(&fresh, &v) != 1 || !Sorted(t, 31)) { std::printf("00c1b720_TEST FAIL\n"); return 1; }
    // every inserted value is reachable at its key
    for (unsigned int i = 1; i <= 31; ++i) {
        Node* n = t.Root;
        while (n != &t.Sentinel && n->Key != i) n = (i < n->Key) ? n->Left : n->Right;
        if (n == &t.Sentinel || n->Value != &marker[i]) { std::printf("00c1b720_TEST FAIL\n"); return 1; }
    }
    std::printf("00c1b720_TEST PASS\n");
    return 0;
}