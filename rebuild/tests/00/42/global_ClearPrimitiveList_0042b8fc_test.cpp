// Behaviour test for 0x0042B8FC (NF229) on the shared headers engine/NUISystem_CManager.h / stlport/fable_map.h / engine/CCountedPointer.h:
// NUISystem::CManager's map<CCharString, CCountedPointer<CComponent> > (BastardChildren). Under test: the tree _M_erase (reached through
// clear()): every node's pair is destroyed (key destructor runs once per node) and freed, for a one-node tree and for a tree whose root
// has a right subtree and a left chain (insert order "M", "T", "C", "A", "P" on the unbalanced tree: the right recursion and the left
// continuation both run); the tree is reusable. Stubbed: the out-of-line CCharString constructors / destructor / operator< (counted; the
// stub keeps the literal's address in PStringData and compares with strcmp, never dereferencing it as string data) and the STLport
// rebalance. The null counted pointer's release is the header's inline ReleaseData.
#define FABLE_RETAIL_CCHARSTRING   // test only: the retail CCharString (PStringData, the out-of-line operator<) as the binder rows 0x0042B7EE / 0x0042B76C use
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
typedef std::map<CCharString, CCountedPointer<NUISystem::CComponent> > ChildMap;
static void Add(ChildMap& m, const char* text) { CCharString key(text, -1); m[key]; }
int main()
{
    {
        ChildMap m;
        Add(m, "Q");
        Check(m.size() == 1 && g_live == 1, "one node: its key copy is live");
        m.clear();
        Check(m.size() == 0 && m.begin() == m.end() && g_live == 0, "erase one node destroys its key");
        Add(m, "M"); Add(m, "T"); Add(m, "C"); Add(m, "A"); Add(m, "P");
        Check(m.size() == 5 && g_live == 5, "five nodes, five live keys");
        m.clear();
        Check(m.size() == 0 && m.begin() == m.end() && g_live == 0, "erase right subtree and left chain destroys every key");
        Add(m, "Z");
        Check(m.size() == 1 && g_live == 1, "reuse after erase");
    }
    Check(g_live == 0, "nothing left");
    printf(g_fails ? "FAIL_0042b8fc\n" : "PASS_0042b8fc\n");
    return g_fails ? 1 : 0;
}
