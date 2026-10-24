// behaviour test for 0x00B93290 (CEngineScene nested map walk)
#include <cstdio>

// STLport-style red-black tree nodes (header: Parent = root, Left = leftmost, Right = rightmost)
struct Grid { int calls; long lastLayer; unsigned long lastA, lastB; bool lastFlag; };
struct GridPtr { Grid* obj; long* count; };
struct SceneMap { unsigned char pad[0x18]; GridPtr* levels; };
struct MapNode { unsigned long c; MapNode *Parent, *Left, *Right; long Key; SceneMap* Data; void* Info; };
struct MapTree { MapNode* Head; unsigned long Count; };
struct OuterNode { unsigned long c; OuterNode *Parent, *Left, *Right; long Key; MapTree Maps; };
template <class T> static T* Next(T* node)
{
    if (node->Right) { node = node->Right; while (node->Left) node = node->Left; }
    else { T* parent = node->Parent; while (node == parent->Right) { node = parent; parent = parent->Parent; } if (node->Right != parent) node = parent; }
    return node;
}
// build: header + root + left child + right child
template <class N> static void Build3(N* h, N* root, N* l, N* r)
{
    h->Parent = root; h->Left = l; h->Right = r;
    root->Parent = h; root->Left = l; root->Right = r;
    l->Parent = root; l->Left = 0; l->Right = 0;
    r->Parent = root; r->Left = 0; r->Right = 0;
}
static Grid g_grids[6];
static GridPtr g_ptrs[6];
static SceneMap g_maps[6];
static MapNode g_inner[2][4];
static OuterNode g_outer[3];
static OuterNode g_head;
static void Setup()
{
    for (int i = 0; i < 6; ++i) { g_grids[i].calls = 0; g_ptrs[i].obj = &g_grids[i]; g_maps[i].levels = &g_ptrs[i]; }
    // outer tree: header + root(outer[0]) with only a left child outer[1]
    g_head.Parent = &g_outer[0]; g_head.Left = &g_outer[1]; g_head.Right = &g_outer[0];
    g_outer[0].Parent = &g_head; g_outer[0].Left = &g_outer[1]; g_outer[0].Right = 0;
    g_outer[1].Parent = &g_outer[0]; g_outer[1].Left = 0; g_outer[1].Right = 0;
    for (int o = 0; o < 2; ++o) {
        MapNode* n = g_inner[o];
        Build3(&n[0], &n[1], &n[2], &n[3]);
        n[1].Data = &g_maps[o * 3 + 0]; n[2].Data = &g_maps[o * 3 + 1]; n[3].Data = &g_maps[o * 3 + 2];
        g_outer[o].Maps.Head = &n[0];
    }
}

struct Prim { unsigned char pad[0x38]; Prim* next; int seen; };
struct Enumerator { int prims; virtual void ProcessPrimitive(Prim& p) { ++p.seen; ++prims; } virtual ~Enumerator() { } };
static void EnumerateScene(OuterNode* head, Prim* moving, Enumerator& e)
{
    for (OuterNode* outer = head->Left; outer != head; outer = Next(outer))
        for (MapNode* inner = outer->Maps.Head->Left; inner != outer->Maps.Head; inner = Next(inner))
            ++inner->Data->levels->obj->calls;
    for (Prim* p = moving; p; p = p->next)
        e.ProcessPrimitive(*p);
}
int main()
{
    Setup();
    Prim a, b; a.next = &b; b.next = 0; a.seen = b.seen = 0;
    Enumerator e; e.prims = 0;
    EnumerateScene(&g_head, &a, e);
    for (int i = 0; i < 6; ++i)
        if (g_grids[i].calls != 1) { std::printf("00b93290_TEST FAIL\n"); return 1; }
    if (e.prims != 2 || a.seen != 1 || b.seen != 1) { std::printf("00b93290_TEST FAIL\n"); return 1; }
    std::printf("00b93290_TEST PASS\n");
    return 0;
}