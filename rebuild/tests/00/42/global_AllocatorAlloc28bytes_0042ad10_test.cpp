// Behaviour test for 0x0042AD10 (NF229) on the shared headers engine/NUISystem_CManager.h / stlport/fable_map.h / engine/CCountedPointer.h:
// NUISystem::CManager's map<CCharString, CCountedPointer<CComponent> > (BastardChildren; 0x1C-byte nodes). Under test: the
// _Rb_tree_base(const allocator&) constructor (reached through the map's default constructor): it allocates a header node of its own for
// each tree (two maps get distinct headers), the new tree is empty (size 0, begin == end), and the header works as the tree's end
// sentinel (one insert, then find hit and miss). One case per condition. Stubbed: the out-of-line CCharString constructors / destructor /
// operator< (the stub keeps the literal's address in PStringData and compares with strcmp) and the STLport rebalance.
#define FABLE_RETAIL_CCHARSTRING   // test only: the retail CCharString (PStringData, the out-of-line operator<), as the binder 0x0042B7EE
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include <stdio.h>
#include <string.h>
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
// Stub of the out-of-line STLport rebalance (a landed row of its own): the tree stays a valid, unbalanced search tree.
namespace std { template<> void _Rb_global<bool>::_Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*&) { } }
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }
static int g_live = 0;
CCharString::CCharString(const char* text, long) { PStringData = (CCharStringData*)text; ++g_live; }
CCharString::CCharString(const CCharString& other) { PStringData = other.PStringData; ++g_live; }
CCharString::~CCharString() { --g_live; }
CCharString::operator const char*() const { return (const char*)PStringData; }
bool CCharString::operator<(const CCharString& other) const { return strcmp((const char*)PStringData, (const char*)other.PStringData) < 0; }
typedef std::map<CCharString, CCountedPointer<NUISystem::CComponent> > Map;
int main()
{
    {
        Map a;
        Map b;
        Check(a.size() == 0 && a.begin() == a.end(), "new tree empty");
        Check(b.size() == 0 && b.begin() == b.end() && !(a.end() == b.end()), "each tree has its own header");
        {
            CCharString root("ROOT", -1);
            a[root];
        }
        CCharString hit("ROOT", -1), miss("LEAF", -1);
        Check(a.size() == 1 && a.find(hit) != a.end() && a.find(miss) == a.end() && b.size() == 0, "the header is the end sentinel");
    }
    Check(g_live == 0, "every key destroyed");
    printf(g_fails ? "ALLOC_28_FAIL\n" : "ALLOC_28_PASS\n");
    return g_fails ? 1 : 0;
}
