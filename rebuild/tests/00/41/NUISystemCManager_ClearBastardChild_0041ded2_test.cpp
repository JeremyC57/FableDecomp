// Behaviour test for 0x0041ded2 (NF230, q12624) on the shared header engine/NUISystem_CManager.h: NUISystem::CManager::ClearBastardChild, the row's
// body under the row's defines, on the header class, called non-virtually on raw manager storage whose touched members are built in
// place (a never-constructed test subclass names the protected members). One case per condition in the row. Test only: FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE left out (the row calls those bodies out of line; the test takes the header's inline bodies). Replaces the HEAD host model.
#define FABLE_RETAIL_STLP_MAP   // test only (with the next two): the STLport map operator[] / insert bodies, as the 0x0042B8FC test
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_CCHARSTRING   // test only: the retail CCharString (PStringData, the out-of-line operator<), as the 0x0042B8FC test
#include <string.h>
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include <stdio.h>
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
// Stubs: the out-of-line STLport rebalance (a landed row of its own) and the out-of-line CCharString constructors / destructor /
// operator< (counted; the stub keeps the literal's address in PStringData and compares with strcmp, never reading it as string data).
namespace std { template<> void _Rb_global<bool>::_Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*&) { } }
static int g_live = 0, g_deleted = 0;
CCharString::CCharString(const char* text, long) { PStringData = (CCharStringData*)text; ++g_live; }
CCharString::CCharString(const CCharString& other) { PStringData = other.PStringData; ++g_live; }
CCharString::~CCharString() { --g_live; }
CCharString::operator const char*() const { return (const char*)PStringData; }
bool CCharString::operator<(const CCharString& other) const { return strcmp((const char*)PStringData, (const char*)other.PStringData) < 0; }
static void FABLE_FASTCALL CountDelete(void*) { ++g_deleted; }
static int g_component;   // stands for a component object (never dereferenced)
namespace NUISystem {
void CManager::ClearBastardChild()
{
    for (std::map<CCharString,CCountedPointer<CComponent> >::iterator it = BastardChildren.begin(); it != BastardChildren.end(); ++it) {
        (*it).second.ReleaseData();
        BastardChild.ReleaseData();
    }
    BastardChildren.clear();
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
        typedef std::map<CCharString,CCountedPointer<NUISystem::CComponent> > ChildMap;
        typedef CCountedPointer<NUISystem::CComponent> ChildPtr;
        ChildMap* children = new (&(g_mgr->*CManagerPeek::BastardChildrenMember())) ChildMap();
        ChildPtr* child = new (&(g_mgr->*CManagerPeek::BastardChildMember())) ChildPtr();
        CCPPointerInfo childInfo(5, CountDelete, 0);
        child->Data = (NUISystem::CComponent*)&g_component; child->Info = &childInfo;
        g_mgr->NUISystem::CManager::ClearBastardChild();
        Check(children->size() == 0 && childInfo.RefCount == 5 && child->Info == &childInfo, "no children: the bastard child is left alone");
        CCPPointerInfo keep(3, CountDelete, 0);
        { CCharString a("A", -1); ChildPtr& p = (*children)[a]; p.Data = (NUISystem::CComponent*)&g_component; p.Info = new CCPPointerInfo(1, CountDelete, 0); }
        { CCharString b("B", -1); ChildPtr& p = (*children)[b]; p.Data = (NUISystem::CComponent*)&g_component; p.Info = &keep; }
        Check(children->size() == 2 && g_live == 2, "set up: two children");
        g_mgr->NUISystem::CManager::ClearBastardChild();
        Check(g_deleted == 1, "a child's last reference deletes it");
        Check(keep.RefCount == 2, "a shared child loses one reference");
        Check(childInfo.RefCount == 4 && child->Data == 0 && child->Info == 0, "the bastard child is released once");
        Check(children->size() == 0 && children->begin() == children->end() && g_live == 0, "the map is cleared, every key destroyed");
        child->~ChildPtr();
        children->~ChildMap();
    }
    printf(g_fails ? "0041ded2_TEST FAIL\n" : "0041ded2_TEST PASS\n");
    return g_fails ? 1 : 0;
}
