// Behaviour test for 0x00B94180 CEngineScene::RenderSceneOcclusion: the occlusion mask, the layer-mask gate, the walk of
// every scene map of every map list entry in key order (STLport tree increment), and the two primitive-list renders.
#include <cstdio>

struct Node { bool color; Node* parent; Node* left; Node* right; long key; void* value; };

static Node* increment(Node* n)
{
    if (n->right != 0) { n = n->right; while (n->left != 0) n = n->left; }
    else { Node* y = n->parent; while (n == y->right) { n = y; y = y->parent; } if (n->right != y) n = y; }
    return n;
}

struct Grid { int id; };
struct SceneMap { Grid* level0; };
struct Stats { int dummy; };

static int g_cells[16], g_cellCount = 0, g_badCall = 0, g_listCalls = 0;
static void* g_lists[4];
static Stats g_stats[8];
static int g_statsLayer = -1;

struct Scene {
    Node* mapListHeader;
    unsigned long MapsRenderLayerMask;
    void* FirstMovingPrimitive;
    void* FirstNonPersistentPrimitive;
    unsigned long CurrentOcclusionMask;
    Stats& RenderStatsAt(int layer) { g_statsLayer = layer; return g_stats[layer]; }
    void RenderCellClippedOcclusion(Grid* g, unsigned long x, unsigned long y, int layer, unsigned long mask, bool clipped, Stats& s)
    {
        if (x != 0 || y != 0 || clipped || mask != CurrentOcclusionMask || &s != &g_stats[layer]) g_badCall = 1;
        g_cells[g_cellCount++] = g->id;
    }
    void RenderPrimitiveList(void* list, int layer, Stats& s) { if (&s != &g_stats[layer]) g_badCall = 1; g_lists[g_listCalls++] = list; }
    void RenderSceneOcclusion(int layer, int occlusion_layer)
    {
        Stats& stats = RenderStatsAt(layer);
        CurrentOcclusionMask = 1 << (occlusion_layer + 1);
        if (MapsRenderLayerMask & layer)
            for (Node* it = mapListHeader->left; it != mapListHeader; it = increment(it)) {
                Node* inner = (Node*)it->value;
                for (Node* m = inner->left; m != inner; m = increment(m))
                    RenderCellClippedOcclusion(((SceneMap*)m->value)->level0, 0, 0, layer, CurrentOcclusionMask, false, stats);
            }
        RenderPrimitiveList(FirstMovingPrimitive, layer, stats);
        RenderPrimitiveList(FirstNonPersistentPrimitive, layer, stats);
    }
};

static void link3(Node* header, Node* a, Node* b, Node* c)    // balanced tree b(a, c) under header
{
    header->parent = b; header->left = a; header->right = c;
    b->parent = header; b->left = a; b->right = c;
    a->parent = b; a->left = a->right = 0;
    c->parent = b; c->left = c->right = 0;
}
static void link1(Node* header, Node* a)
{
    header->parent = a; header->left = a; header->right = a;
    a->parent = header; a->left = a->right = 0;
}

int main()
{
    Grid grids[4] = { { 10 }, { 11 }, { 12 }, { 20 } };
    SceneMap maps[4] = { { &grids[0] }, { &grids[1] }, { &grids[2] }, { &grids[3] } };
    Node innerA_h, innerA[3], innerB_h, innerB[1], outer_h, outer[2];
    for (int i = 0; i < 3; ++i) { innerA[i].key = i; innerA[i].value = &maps[i]; }
    innerB[0].key = 0; innerB[0].value = &maps[3];
    link3(&innerA_h, &innerA[0], &innerA[1], &innerA[2]);
    link1(&innerB_h, &innerB[0]);
    outer[0].key = 1; outer[0].value = &innerA_h;
    outer[1].key = 2; outer[1].value = &innerB_h;
    outer_h.parent = &outer[0]; outer_h.left = &outer[0]; outer_h.right = &outer[1];
    outer[0].parent = &outer_h; outer[0].left = 0; outer[0].right = &outer[1];
    outer[1].parent = &outer[0]; outer[1].left = outer[1].right = 0;

    int listA = 0, listB = 0;
    Scene s; s.mapListHeader = &outer_h; s.MapsRenderLayerMask = 0x6; s.FirstMovingPrimitive = &listA;
    s.FirstNonPersistentPrimitive = &listB; s.CurrentOcclusionMask = 0;
    s.RenderSceneOcclusion(2, 1);
    int bad = s.CurrentOcclusionMask != 4 || g_statsLayer != 2 || g_cellCount != 4 || g_cells[0] != 10 || g_cells[1] != 11
              || g_cells[2] != 12 || g_cells[3] != 20 || g_listCalls != 2 || g_lists[0] != &listA || g_lists[1] != &listB || g_badCall;
    g_cellCount = 0; g_listCalls = 0;
    s.RenderSceneOcclusion(1, 2);                                // layer not in the grid mask: lists only
    bad |= s.CurrentOcclusionMask != 8 || g_statsLayer != 1 || g_cellCount != 0 || g_listCalls != 2 || g_badCall;
    if (bad) { std::printf("00b94180_TEST FAIL\n"); return 1; }
    std::printf("00b94180_TEST PASS\n");
    return 0;
}