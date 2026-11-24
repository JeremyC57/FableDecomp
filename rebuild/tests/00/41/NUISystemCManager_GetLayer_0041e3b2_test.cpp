// Behaviour test for 0x0041e3b2 (NF230, q12624) on the shared header engine/NUISystem_CManager.h: NUISystem::CManager::GetLayer, the row's
// body under the row's defines, on the header class, called non-virtually on raw manager storage whose touched members are built in
// place (a never-constructed test subclass names the protected members). One case per condition in the row. Test only: FABLE_RETAIL_STLP_TREE_MFIND_DECL left out (the row calls those bodies out of line; the test takes the header's inline bodies). Replaces the HEAD host model.
#define FABLE_RETAIL_STLP_MAP   // test only (with the next two): the STLport map operator[] / insert bodies, as the 0x0042B8FC test
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY

#include <stdio.h>
#include "engine/NUISystem_CManager.h"
// Stub of the out-of-line STLport rebalance (a landed row of its own): the tree stays a valid, unbalanced search tree.
namespace std { template<> void _Rb_global<bool>::_Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*&) { } }
namespace NUISystem {
EEngine2DLayer CManager::GetLayer(long key)
{
    if (key < -3)
        key = -3;
    else if (key > 13)
        key = 13;
    std::map<long,EEngine2DLayer>::iterator it = Layers.find(key);
    if (it != Layers.end())
        return (*it).second;
    key = 0;
    return Layers[key];
}
}
// Test-only access to the protected members through a derived class (never constructed: the members a case needs are built in place).
struct CManagerPeek : NUISystem::CManager {
    static bool NUISystem::CManager::* MouseDisabledMember() { return &CManagerPeek::MouseDisabled; }
    static C2DVector NUISystem::CManager::* RightStickForceMember() { return &CManagerPeek::RightStickForce; }
    static std::map<long,EEngine2DLayer> NUISystem::CManager::* LayersMember() { return &CManagerPeek::Layers; }
    static CCountedPointer<NUISystem::CComponent> NUISystem::CManager::* BastardChildMember() { return &CManagerPeek::BastardChild; }
    static std::map<CCharString,CCountedPointer<NUISystem::CComponent> > NUISystem::CManager::* BastardChildrenMember() { return &CManagerPeek::BastardChildren; }
};
static double g_store[(sizeof(NUISystem::CManager) + 7) / 8];   // raw storage for the manager (its constructor is protected and out of line)
static NUISystem::CManager* const g_mgr = (NUISystem::CManager*)g_store;
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }

int main()
{
    {
        typedef std::map<long,EEngine2DLayer> LayerMap;
        LayerMap* layers = new (&(g_mgr->*CManagerPeek::LayersMember())) LayerMap();
        long k;
        k = 5;  (*layers)[k] = (EEngine2DLayer)50;
        k = -3; (*layers)[k] = (EEngine2DLayer)30;
        k = 13; (*layers)[k] = (EEngine2DLayer)130;
        Check(g_mgr->NUISystem::CManager::GetLayer(5) == (EEngine2DLayer)50, "a present key: its layer");
        Check(g_mgr->NUISystem::CManager::GetLayer(-9) == (EEngine2DLayer)30, "below -3: clamped to -3");
        Check(g_mgr->NUISystem::CManager::GetLayer(40) == (EEngine2DLayer)130, "above 13: clamped to 13");
        Check(layers->size() == 3, "found keys insert nothing");
        Check(g_mgr->NUISystem::CManager::GetLayer(7) == (EEngine2DLayer)0 && layers->size() == 4 && layers->find(0) != layers->end(),
              "an absent key falls back to key 0, inserted with the default layer");
        k = 0; (*layers)[k] = (EEngine2DLayer)70;
        Check(g_mgr->NUISystem::CManager::GetLayer(8) == (EEngine2DLayer)70 && layers->size() == 4, "an absent key reads key 0's layer");
        layers->~LayerMap();
    }
    printf(g_fails ? "0041e3b2_TEST FAIL\n" : "0041e3b2_TEST PASS\n");
    return g_fails ? 1 : 0;
}
