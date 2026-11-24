// Behaviour test for 0x0041E1CD NUISystem::CManager::SetMetaLayer (NF228) on engine/NUISystem_CManager.h. Links against the row object
// (verify_and_land's second link: the test object leaves SetMetaLayer undefined), so the function under test is the row's own; it runs on
// raw storage of the header's size whose Layers map (the header's map<long,EEngine2DLayer>) is constructed in place at the header offset.
// Defined here: the per-meta-layer base table 0x01230118 (test values), the STLport rebalance stub; the map's out-of-line insert bodies
// come from this test's own use of the map (same STLport COMDATs the row object calls).
// Cases: meta layer 0 (the fixed table, keys -3..13), meta layers 1..4 (base + key + 2 for keys -3..7), clamping below 0 and at >= 5,
// the stored meta layer, and an existing key overwritten (no second node).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include <stdio.h>
#include <stddef.h>
#include <new>
#include "engine/NUISystem_CManager.h"
namespace std { template<> void _Rb_global<bool>::_Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*&) { } }
long g_metaLayerBase[5] = { 0, 100, 200, 300, 400 };   // 0x01230118 (test values)
typedef std::map<long, EEngine2DLayer> LayerMap;
struct CManagerOffsets : NUISystem::CManager {        // never instantiated: names the protected members' offsets
    static size_t LayersOffset() { return offsetof(CManagerOffsets, Layers); }
    static size_t MetaLayerOffset() { return offsetof(CManagerOffsets, MetaLayer); }
};
static double g_store[sizeof(NUISystem::CManager) / sizeof(double) + 1];
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }
static NUISystem::CManager* Manager() { return reinterpret_cast<NUISystem::CManager*>(g_store); }
static LayerMap& Layers() { return *reinterpret_cast<LayerMap*>(reinterpret_cast<char*>(g_store) + CManagerOffsets::LayersOffset()); }
static long MetaLayer() { return *reinterpret_cast<long*>(reinterpret_cast<char*>(g_store) + CManagerOffsets::MetaLayerOffset()); }
static void Run(long meta) { Manager()->NUISystem::CManager::SetMetaLayer(meta); }
int main()
{
    new (&Layers()) LayerMap();
    { LayerMap warm; warm[1] = ENGINE_2D_LAYER_1; }      // instantiates the out-of-line insert bodies here
    Run(0);
    static const long fixed[17] = { 0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51 };
    bool ok = MetaLayer() == 0 && Layers().size() == 17;
    for (long k = -3; k <= 13; ++k) ok = ok && Layers()[k] == (EEngine2DLayer)fixed[k + 3];
    Check(ok, "meta layer 0: fixed table for -3..13");
    for (long m = 1; m <= 4; ++m) {
        Run(m);
        ok = MetaLayer() == m && Layers().size() == 17;
        for (long k = -3; k <= 7; ++k) ok = ok && Layers()[k] == (EEngine2DLayer)(g_metaLayerBase[m] + k + 2);
        ok = ok && Layers()[13] == (EEngine2DLayer)0x51;  // keys 8..13 keep the earlier values
        Check(ok, "meta layers 1..4: base + key + 2 for -3..7");
    }
    Run(-5);
    Check(MetaLayer() == 0 && Layers()[0] == (EEngine2DLayer)0x43, "below 0 clamps to 0");
    Run(9);
    Check(MetaLayer() == 4 && Layers()[0] == (EEngine2DLayer)(400 + 2), "5 and above clamp to 4");
    Run(5);
    Check(MetaLayer() == 4 && Layers().size() == 17, "5 clamps to 4, no new nodes");
    Layers().~LayerMap();
    printf(g_fails ? "0041e1cd_TEST FAIL\n" : "0041e1cd_TEST PASS\n");
    return g_fails ? 1 : 0;
}
