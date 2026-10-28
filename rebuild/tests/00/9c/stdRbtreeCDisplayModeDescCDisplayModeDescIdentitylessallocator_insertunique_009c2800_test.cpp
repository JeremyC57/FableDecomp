// Behaviour test for 0x009C2800: STLport _Rb_tree<CDisplayModeDesc,...>::insert_unique -- descend by the lexicographic
// (width, height, bits per pixel, refresh rate) order, then reject a key equal to its in-order predecessor.
// The test tree is unbalanced (insert_unique's search and uniqueness logic does not depend on the colouring).
#include <cstdio>

class CDisplayModeDesc { public:
    long X, Y, BitsPerPixel, RefreshRate;
    bool operator<(const CDisplayModeDesc& o) const
    {
        if (X != o.X) return X < o.X;
        if (Y != o.Y) return Y < o.Y;
        if (BitsPerPixel != o.BitsPerPixel) return BitsPerPixel < o.BitsPerPixel;
        return RefreshRate < o.RefreshRate;
    }
};
struct Node { Node* Parent; Node* Left; Node* Right; CDisplayModeDesc Value; };
static Node g_nodes[16];
static int  g_used;
static Node g_header;                                     // Parent = root, Left = leftmost, Right = rightmost

static Node* Prev(Node* x)                                // _Rb_tree_iterator::operator-- (non-header node)
{
    if (x->Left) { x = x->Left; while (x->Right) x = x->Right; return x; }
    Node* y = x->Parent;
    while (x == y->Left) { x = y; y = y->Parent; }
    return y;
}
static Node* Insert(Node* y, const CDisplayModeDesc& v, bool left)
{
    Node* z = &g_nodes[g_used++];
    z->Value = v; z->Left = z->Right = 0; z->Parent = y;
    if (y == &g_header) { g_header.Parent = g_header.Left = g_header.Right = z; }
    else if (left) { y->Left = z; if (y == g_header.Left) g_header.Left = z; }
    else { y->Right = z; if (y == g_header.Right) g_header.Right = z; }
    return z;
}
static bool InsertUnique(const CDisplayModeDesc& v, Node** where)
{
    Node* y = &g_header;
    Node* x = g_header.Parent;
    bool comp = true;
    while (x != 0) {
        y = x;
        comp = v < x->Value;
        x = comp ? x->Left : x->Right;
    }
    Node* j = y;
    if (comp) {
        if (j == g_header.Left) { *where = Insert(y, v, true); return true; }
        j = Prev(j);
    }
    if (j->Value < v) { *where = Insert(y, v, comp || y == &g_header); return true; }
    *where = j;
    return false;
}

int main()
{
    g_header.Parent = 0; g_header.Left = g_header.Right = &g_header;
    const CDisplayModeDesc modes[] = {
        { 1024, 768, 32, 60 }, { 800, 600, 32, 60 }, { 1024, 768, 16, 60 }, { 1024, 768, 32, 75 },
        { 800, 600, 32, 60 }, { 1280, 1024, 32, 60 }, { 1024, 768, 32, 60 },
    };
    const bool fresh[] = { true, true, true, true, false, true, false };
    Node* where;
    for (int i = 0; i < 7; ++i)
        if (InsertUnique(modes[i], &where) != fresh[i] || where->Value < modes[i] || modes[i] < where->Value) { std::printf("009c2800_TEST FAIL\n"); return 1; }
    if (g_used != 5 || g_header.Left->Value.X != 800 || g_header.Right->Value.X != 1280) { std::printf("009c2800_TEST FAIL\n"); return 1; }
    std::printf("009c2800_TEST PASS\n");
    return 0;
}