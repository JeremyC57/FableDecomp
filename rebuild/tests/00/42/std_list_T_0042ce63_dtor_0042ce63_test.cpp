// Behaviour test for 0x0042CE63 (NF228, test only: the landed source is unchanged) on the shared headers engine/NUISystem_CManager.h / stlport/fable_map.h / engine/CCountedPointer.h:
// NUISystem::CManager's map<CCharString, CCountedPointer<CComponent> > (BastardChildren). Under test: the tree destructor ~_Rb_tree (clear, then the header node freed): a non-empty map going out of scope destroys every node. Also: the tree clear(): an empty tree stays empty, a non-empty one ends empty and each node value (key, counted pointer) is destroyed.
// Stubbed: the out-of-line CCharString constructors / destructor (counted; they hold no text here, so the tree gets one key, which
// needs no comparison) and the STLport rebalance. The null counted pointer's release is the header's inline ReleaseData.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include <stdio.h>
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
namespace std { template<> void _Rb_global<bool>::_Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*&) { } }
static int g_live = 0;
CCharString::CCharString(const char*, long) { ++g_live; }
CCharString::CCharString(const CCharString&) { ++g_live; }
CCharString::~CCharString() { --g_live; }
CCharString::operator const char*() const { return ""; }   // referenced by less<CCharString>; never reached with one key
typedef std::map<CCharString, CCountedPointer<NUISystem::CComponent> > ChildMap;
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }
int main()
{
    {
        ChildMap m;
        Check(m.size() == 0 && m.begin() == m.end(), "new map empty");
        m.clear();
        Check(m.size() == 0 && m.begin() == m.end(), "clear empty");
        {
            CCharString key("BASTARD", -1);
            m[key];
        }
        Check(m.size() == 1 && g_live == 1, "one node: its key copy is live");
        m.clear();
        Check(m.size() == 0 && m.begin() == m.end() && g_live == 0, "clear non-empty destroys the node's key");
    }
    Check(g_live == 0, "nothing left");
    {
        ChildMap d;
        { CCharString key("DTOR", -1); d[key]; }
        Check(d.size() == 1 && g_live == 1, "dtor case: one live node");
    }
    Check(g_live == 0, "~_Rb_tree destroys the node's key");
    printf(g_fails ? "0042ce63_TEST FAIL\n" : "0042ce63_TEST PASS\n");
    return g_fails ? 1 : 0;
}
