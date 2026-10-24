// Behaviour test for 00c1ae50: PKCTRRedblackMap::Find -- binary search by unsigned key: 0 + value + LastFound on a hit, 3 on a miss
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
    long Find(const unsigned int* key, void** value)
    {
        Node* node = Root;
        while (node != &Sentinel) {
            if (*key == node->Key) {
                *value = node->Value;
                LastFound = node;
                return 0;
            }
            if (*key < node->Key)
                node = node->Left;
            else
                node = node->Right;
        }
        return 3;
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
    Node* nodes[7];
    for (int i = 0; i < 7; ++i) nodes[i] = t.Add(order[i]);
    for (int i = 0; i < 7; ++i) {
        void* value = 0;
        if (t.Find(&order[i], &value) != 0 || value != nodes[i]->Value || t.LastFound != nodes[i]) { std::printf("00c1ae50_TEST FAIL\n"); return 1; }
    }
    unsigned int missing = 35;
    void* value = (void*)&t;
    t.LastFound = nodes[0];
    if (t.Find(&missing, &value) != 3 || value != (void*)&t || t.LastFound != nodes[0]) { std::printf("00c1ae50_TEST FAIL\n"); return 1; }   // miss: 3, nothing written
    Tree empty;
    if (empty.Find(&missing, &value) != 3) { std::printf("00c1ae50_TEST FAIL\n"); return 1; }
    std::printf("00c1ae50_TEST PASS\n");
    return 0;
}