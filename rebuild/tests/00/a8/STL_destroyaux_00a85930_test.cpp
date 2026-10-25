// Behaviour test for 0x00A85930 _STL::__destroy_aux<CNavQuadTree::CNavigationLayer*>: destroying a range of layers
// destroys, per layer, the switchable map's lists then frees its storage, the line lists then their storage, then the
// node array (members in reverse order).
#include <cstdio>
#include <cstdlib>

static int g_log[64];
static int g_n = 0;
static void Free(void* p, int tag) { if (p) { g_log[g_n++] = tag; std::free(p); } }

struct CList { int id; ~CList() { g_log[g_n++] = id; } };
struct CVecList { CList* start; CList* finish; };
struct CPairList { unsigned int lo, hi; CList second; };
struct CVecPair { CPairList* start; CPairList* finish; };
struct CVecNode { void** start; };
struct CLayer { CVecNode Nodes; CVecList LineLists; CVecPair SwitchableLineLists; };

static void DestroyLayers(CLayer* first, CLayer* last)
{
    for (; first != last; ++first) {
        for (CPairList* p = first->SwitchableLineLists.start; p != first->SwitchableLineLists.finish; ++p) p->second.~CList();
        Free(first->SwitchableLineLists.start, 900);
        for (CList* l = first->LineLists.start; l != first->LineLists.finish; ++l) l->~CList();
        Free(first->LineLists.start, 800);
        Free(first->Nodes.start, 700);
    }
}

int main()
{
    CLayer layers[2];
    for (int i = 0; i < 2; ++i) {
        layers[i].SwitchableLineLists.start = (CPairList*)std::malloc(sizeof(CPairList) * 2);
        layers[i].SwitchableLineLists.finish = layers[i].SwitchableLineLists.start + 1 + i;
        for (int j = 0; j < 1 + i; ++j) layers[i].SwitchableLineLists.start[j].second.id = 10 * (i + 1) + j;
        layers[i].LineLists.start = (CList*)std::malloc(sizeof(CList));
        layers[i].LineLists.finish = layers[i].LineLists.start + 1;
        layers[i].LineLists.start[0].id = 50 + i;
        layers[i].Nodes.start = i ? (void**)std::malloc(8) : 0;
    }
    DestroyLayers(layers, layers + 2);
    static const int expect[] = { 10, 900, 50, 800, 20, 21, 900, 51, 800, 700 };
    int ok = g_n == 10;
    for (int k = 0; ok && k < 10; ++k) ok = g_log[k] == expect[k];
    std::printf(ok ? "00a85930_TEST PASS\n" : "00a85930_TEST FAIL\n");
    return ok ? 0 : 1;
}