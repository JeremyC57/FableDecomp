// Behaviour test for 00c18b20: PKCTRRedblackTree InsertFixup -- after each plain BST insert of a red node, the fixup (recolour / rotate through the nil sentinel) must leave a valid red-black tree
#include <cstdio>

struct RbNode {
    RbNode* Left;
    RbNode* Right;
    RbNode* Parent;
    long    Colour;
    void* Key;
    long  KeyValue;
};
enum { BLACK = 0, RED = 1 };

class Tree {
public:
    virtual ~Tree() { }
    void* PAllocator;
    void* POwnedAllocator;
    RbNode Sentinel;
    RbNode* Root;
    long Count;
    Tree() : Root(&Sentinel), Count(0)
    {
        Sentinel.Left = Sentinel.Right = &Sentinel;
        Sentinel.Parent = 0;
        Sentinel.Colour = BLACK;
    }
    void RotateLeft(RbNode* x)
    {
        RbNode* y = x->Right;
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
    void RotateRight(RbNode* x)
    {
        RbNode* y = x->Left;
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
    void InsertFixup(RbNode* x)
    {
        while (x != Root && x->Parent->Colour == RED) {
            if (x->Parent == x->Parent->Parent->Left) {
                RbNode* y = x->Parent->Parent->Right;
                if (y->Colour == RED) {
                    x->Parent->Colour = BLACK;
                    y->Colour = BLACK;
                    x->Parent->Parent->Colour = RED;
                    x = x->Parent->Parent;
                } else {
                    if (x == x->Parent->Right) {
                        x = x->Parent;
                        RotateLeft(x);
                    }
                    x->Parent->Colour = BLACK;
                    x->Parent->Parent->Colour = RED;
                    RotateRight(x->Parent->Parent);
                }
            } else {
                RbNode* y = x->Parent->Parent->Left;
                if (y->Colour == RED) {
                    x->Parent->Colour = BLACK;
                    y->Colour = BLACK;
                    x->Parent->Parent->Colour = RED;
                    x = x->Parent->Parent;
                } else {
                    if (x == x->Parent->Left) {
                        x = x->Parent;
                        RotateRight(x);
                    }
                    x->Parent->Colour = BLACK;
                    x->Parent->Parent->Colour = RED;
                    RotateLeft(x->Parent->Parent);
                }
            }
        }
        Root->Colour = BLACK;
    }
    void Insert(RbNode* n)                         // the caller's descent (retail insert), then the fixup
    {
        RbNode* parent = 0;
        RbNode* cur = Root;
        while (cur != &Sentinel) { parent = cur; cur = n->KeyValue < cur->KeyValue ? cur->Left : cur->Right; }
        n->Parent = parent;
        n->Left = n->Right = &Sentinel;
        n->Colour = RED;
        if (parent) { if (n->KeyValue < parent->KeyValue) parent->Left = n; else parent->Right = n; }
        else Root = n;
        ++Count;
        InsertFixup(n);
    }
    // returns the black height, or -1 on a violation
    int Check(RbNode* n, RbNode* parent, long lo, long hi, int* seen) const
    {
        if (n == &Sentinel) return 1;
        if (n->Parent != parent) return -1;
        if (n->KeyValue < lo || n->KeyValue > hi) return -1;
        if (n->Colour == RED && (n->Left->Colour == RED || n->Right->Colour == RED)) return -1;
        ++*seen;
        int l = Check(n->Left, n, lo, n->KeyValue, seen);
        int r = Check(n->Right, n, n->KeyValue, hi, seen);
        if (l < 0 || r < 0 || l != r) return -1;
        return l + (n->Colour == BLACK ? 1 : 0);
    }
    bool Valid() const
    {
        int seen = 0;
        if (Root != &Sentinel && (Root->Colour != BLACK || Root->Parent != 0)) return false;
        if (Sentinel.Colour != BLACK) return false;
        return Check(Root, 0, -100000, 100000, &seen) > 0 && seen == Count;
    }
};

int main()
{
    bool ok = true;
    {
        Tree t;                                    // ascending keys: every insert rotates left at some level
        static RbNode nodes[64];
        for (int i = 0; i < 64; ++i) { nodes[i].KeyValue = i; t.Insert(&nodes[i]); ok = ok && t.Valid(); }
    }
    {
        Tree t;                                    // descending keys: the mirror arms
        static RbNode nodes[64];
        for (int i = 0; i < 64; ++i) { nodes[i].KeyValue = 1000 - i; t.Insert(&nodes[i]); ok = ok && t.Valid(); }
    }
    {
        Tree t;                                    // pseudo-random keys with duplicates
        static RbNode nodes[200];
        unsigned int seed = 12345u;
        for (int i = 0; i < 200; ++i) {
            seed = seed * 1103515245u + 12345u;
            nodes[i].KeyValue = (long)((seed >> 16) % 500u);
            t.Insert(&nodes[i]);
            ok = ok && t.Valid();
        }
    }
    {
        Tree t;
        RbNode one;
        one.KeyValue = 7;
        t.Insert(&one);                            // x == root on entry: just blackened
        ok = ok && t.Root == &one && one.Colour == BLACK && t.Valid();
    }
    if (!ok) { std::printf("00c18b20_TEST FAIL\n"); return 1; }
    std::printf("00c18b20_TEST PASS\n");
    return 0;
}