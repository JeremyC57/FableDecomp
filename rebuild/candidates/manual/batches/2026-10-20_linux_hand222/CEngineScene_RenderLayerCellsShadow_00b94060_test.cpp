// Behaviour test for 0x00B94060 CEngineScene::RenderLayerCells(Shadow) (model copy: nested map walk + two list renders).
#include <cstdio>

struct CMapNode { unsigned long Colour; CMapNode *Parent, *Left, *Right; long Key; int* Data; };
struct COuterNode { unsigned long Colour; COuterNode *Parent, *Left, *Right; long Key; CMapNode* InnerHead; };
template <class T> static T* Next(T* node)
{
    if (node->Right) { node = node->Right; while (node->Left) node = node->Left; }
    else { T* parent = node->Parent; while (node == parent->Right) { node = parent; parent = parent->Parent; } if (node->Right != parent) node = parent; }
    return node;
}
static int g_cells = 0, g_lists = 0, g_stats[8];
struct CScene {
    COuterNode* MapListHead; unsigned long GridLayerMask; long CurrentCell;
    void Render(long layer)
    {
        int& stats = g_stats[layer];
        CurrentCell = -1;
        if (GridLayerMask & layer)
            for (COuterNode* o = MapListHead->Left; o != MapListHead; o = Next(o))
                for (CMapNode* i = o->InnerHead->Left; i != o->InnerHead; i = Next(i)) { ++*i->Data; ++stats; ++g_cells; }
        g_lists += 2;
    }
};

int main()
{
    // outer tree: header + one node; inner tree: header + two nodes (root and its right child)
    COuterNode oh, on; CMapNode ih, r, c; int d1 = 0, d2 = 0;
    ih.Parent = &r; ih.Left = &r; ih.Right = &c;
    r.Parent = &ih; r.Left = 0; r.Right = &c; r.Data = &d1;
    c.Parent = &r; c.Left = 0; c.Right = 0; c.Data = &d2;
    oh.Parent = &on; oh.Left = &on; oh.Right = &on;
    on.Parent = &oh; on.Left = 0; on.Right = 0; on.InnerHead = &ih;
    CScene s; s.MapListHead = &oh; s.GridLayerMask = 2; s.CurrentCell = 5;
    s.Render(2);
    if (d1 != 1 || d2 != 1 || g_cells != 2 || g_stats[2] != 2 || s.CurrentCell != -1 || g_lists != 2) { std::printf("00b94060_TEST FAIL\n"); return 1; }
    s.Render(4);
    if (g_cells != 2 || g_lists != 4) { std::printf("00b94060_TEST FAIL\n"); return 1; }
    std::printf("00b94060_TEST PASS\n");
    return 0;
}
