// Behaviour test for 0x0042AB01 (NF228) on the shared headers engine/NUISystem_CAction.h / stlport/fable_containers.h: the STLport
// _List_base<NUISystem::CActionParam*>(const allocator&) constructor of list<CActionParam*> (CAction::UIActionParams): a new list's
// sentinel node is allocated and linked to itself (empty: begin == end), each list gets its own sentinel, and the list is usable.
#define FABLE_RETAIL_STLP_LIST
#include <stdio.h>
#include "engine/NUISystem_CAction.h"
typedef std::list<NUISystem::CActionParam*> ParamList;
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }
int main()
{
    {
        ParamList a;
        ParamList b;
        Check(a.begin() == a.end(), "new list empty");
        Check(b.begin() == b.end() && !(a.end() == b.end()), "separate sentinels");
        a.push_back((NUISystem::CActionParam*)0);
        Check(!(a.begin() == a.end()) && *a.begin() == 0, "usable: one element");
        Check(b.begin() == b.end(), "the other list untouched");
    }
    printf(g_fails ? "UNSEEN_SMALL_FAIL\n" : "UNSEEN_SMALL_PASS\n");
    return g_fails ? 1 : 0;
}
