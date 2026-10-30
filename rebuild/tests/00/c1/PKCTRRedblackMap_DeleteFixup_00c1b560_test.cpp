// Behaviour test for libpk PKCTRRedblackMap::DeleteFixup (0x00C1B560): a standalone copy of the tree (nil sentinel, the landed rotations
// and InsertFixup, Niemann's deleteNode) drives DeleteFixup through many inserts and deletes and checks the red-black invariants.
#include <cstdio>
#include <cstdlib>
enum { BLACK = 0, RED = 1 };
struct Node { Node* Left; Node* Right; Node* Parent; long Colour; long Key; };
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed at line %d\n", __LINE__); } } while (0)
struct Tree {
    Node Sentinel; Node* Root;
    Tree() { Sentinel.Left = Sentinel.Right = &Sentinel; Sentinel.Parent = 0; Sentinel.Colour = BLACK; Sentinel.Key = 0; Root = &Sentinel; }
    void RotateLeft(Node* x)
    {
        Node* y = x->Right; x->Right = y->Left;
        if (y->Left != &Sentinel) y->Left->Parent = x;
        if (y != &Sentinel) y->Parent = x->Parent;
        if (x->Parent) { if (x == x->Parent->Left) x->Parent->Left = y; else x->Parent->Right = y; } else Root = y;
        y->Left = x; if (x != &Sentinel) x->Parent = y;
    }
    void RotateRight(Node* x)
    {
        Node* y = x->Left; x->Left = y->Right;
        if (y->Right != &Sentinel) y->Right->Parent = x;
        if (y != &Sentinel) y->Parent = x->Parent;
        if (x->Parent) { if (x == x->Parent->Right) x->Parent->Right = y; else x->Parent->Left = y; } else Root = y;
        y->Right = x; if (x != &Sentinel) x->Parent = y;
    }
    void InsertFixup(Node* x)
    {
        while (x != Root && x->Parent->Colour == RED) {
            if (x->Parent == x->Parent->Parent->Left) {
                Node* y = x->Parent->Parent->Right;
                if (y->Colour == RED) { x->Parent->Colour = BLACK; y->Colour = BLACK; x->Parent->Parent->Colour = RED; x = x->Parent->Parent; }
                else { if (x == x->Parent->Right) { x = x->Parent; RotateLeft(x); } x->Parent->Colour = BLACK; x->Parent->Parent->Colour = RED; RotateRight(x->Parent->Parent); }
            } else {
                Node* y = x->Parent->Parent->Left;
                if (y->Colour == RED) { x->Parent->Colour = BLACK; y->Colour = BLACK; x->Parent->Parent->Colour = RED; x = x->Parent->Parent; }
                else { if (x == x->Parent->Left) { x = x->Parent; RotateRight(x); } x->Parent->Colour = BLACK; x->Parent->Parent->Colour = RED; RotateLeft(x->Parent->Parent); }
            }
        }
        Root->Colour = BLACK;
    }
    // the row under test (same text as the source)
    void DeleteFixup(Node* x)
    {
        while (x != Root && x->Colour == BLACK) {
            if (x == x->Parent->Left) {
                Node* w = x->Parent->Right;
                if (w->Colour == RED) { w->Colour = BLACK; x->Parent->Colour = RED; RotateLeft(x->Parent); w = x->Parent->Right; }
                if (w->Left->Colour == BLACK && w->Right->Colour == BLACK) { w->Colour = RED; x = x->Parent; }
                else {
                    if (w->Right->Colour == BLACK) { w->Left->Colour = BLACK; w->Colour = RED; RotateRight(w); w = x->Parent->Right; }
                    w->Colour = x->Parent->Colour; x->Parent->Colour = BLACK; w->Right->Colour = BLACK; RotateLeft(x->Parent); x = Root;
                }
            } else {
                Node* w = x->Parent->Left;
                if (w->Colour == RED) { w->Colour = BLACK; x->Parent->Colour = RED; RotateRight(x->Parent); w = x->Parent->Left; }
                if (w->Right->Colour == BLACK && w->Left->Colour == BLACK) { w->Colour = RED; x = x->Parent; }
                else {
                    if (w->Left->Colour == BLACK) { w->Right->Colour = BLACK; w->Colour = RED; RotateLeft(w); w = x->Parent->Left; }
                    w->Colour = x->Parent->Colour; x->Parent->Colour = BLACK; w->Left->Colour = BLACK; RotateRight(x->Parent); x = Root;
                }
            }
        }
        x->Colour = BLACK;
    }
    void Insert(Node* z, long key)
    {
        Node* y = 0; Node* x = Root;
        while (x != &Sentinel) { y = x; x = key < x->Key ? x->Left : x->Right; }
        z->Key = key; z->Parent = y; z->Left = z->Right = &Sentinel; z->Colour = RED;
        if (!y) Root = z; else if (key < y->Key) y->Left = z; else y->Right = z;
        InsertFixup(z);
    }
    void Delete(Node* z)   // Niemann deleteNode (copies the key into z when it has two children)
    {
        Node* x; Node* y;
        if (z->Left == &Sentinel || z->Right == &Sentinel) y = z;
        else { y = z->Right; while (y->Left != &Sentinel) y = y->Left; }
        x = (y->Left != &Sentinel) ? y->Left : y->Right;
        x->Parent = y->Parent;
        if (y->Parent) { if (y == y->Parent->Left) y->Parent->Left = x; else y->Parent->Right = x; } else Root = x;
        if (y != z) z->Key = y->Key;
        if (y->Colour == BLACK) DeleteFixup(x);
        y->Left = y->Right = 0;   // freed
    }
    Node* Find(long key) { Node* x = Root; while (x != &Sentinel) { if (key == x->Key) return x; x = key < x->Key ? x->Left : x->Right; } return 0; }
};
static int g_count;
static int BlackHeight(Tree& t, Node* n, long lo, long hi)
{
    if (n == &t.Sentinel) return 1;
    g_count++;
    if (n->Key < lo || n->Key > hi) { g_fail = 1; return 0; }
    if (n->Colour == RED && (n->Left->Colour == RED || n->Right->Colour == RED)) { g_fail = 1; std::printf("red-red\n"); }
    if (n->Left != &t.Sentinel && n->Left->Parent != n) g_fail = 1;
    if (n->Right != &t.Sentinel && n->Right->Parent != n) g_fail = 1;
    int a = BlackHeight(t, n->Left, lo, n->Key), b = BlackHeight(t, n->Right, n->Key, hi);
    if (a != b) { g_fail = 1; std::printf("black height\n"); }
    return a + (n->Colour == BLACK ? 1 : 0);
}
static Node g_nodes[600];
int main()
{
    Tree t; unsigned int seed = 12345; int live[600]; int nlive = 0;
    for (int i = 0; i < 600; i++) { seed = seed * 1103515245u + 12345u; t.Insert(&g_nodes[i], (long)((seed >> 8) % 100000)); }
    g_count = 0; BlackHeight(t, t.Root, -1, 100000); CHECK(g_count == 600); CHECK(t.Root->Colour == BLACK);
    for (int i = 0; i < 600; i++) live[nlive++] = (int)g_nodes[i].Key;
    for (int round = 0; round < 450; round++) {
        seed = seed * 1103515245u + 12345u;
        int k = (int)((seed >> 8) % (unsigned int)nlive);
        Node* z = t.Find(live[k]); CHECK(z != 0); if (!z) break;
        t.Delete(z); live[k] = live[--nlive];
        g_count = 0; BlackHeight(t, t.Root, -1, 100000); CHECK(g_count == nlive);
        CHECK(t.Root == &t.Sentinel || t.Root->Colour == BLACK); CHECK(t.Sentinel.Colour == BLACK);
        if (g_fail) break;
    }
    if (g_fail) { std::printf("00c1b560_TEST FAIL\n"); return 1; }
    std::printf("00c1b560_TEST PASS\n");
    return 0;
}