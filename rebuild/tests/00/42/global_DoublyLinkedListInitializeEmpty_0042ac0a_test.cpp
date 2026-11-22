// Behaviour test for 0x0042AC0A (NF229) on the shared headers engine/NUISystem_CObservable.h / stlport/fable_containers.h: the STLport
// _List_base<NUISystem::CObserver*>(const allocator&) constructor of list<CObserver*> (CObservable::Observers /
// ConcurrentExclusiveObservers): a new list's sentinel node is allocated and linked to itself (empty: begin == end), each list gets its
// own sentinel, and the list is usable (push_back, then the other list untouched). One case per condition; no stub, no model.
#define FABLE_RETAIL_STLP_LIST
#include <stdio.h>
#include "engine/NUISystem_CObservable.h"
typedef std::list<NUISystem::CObserver*> ObserverList;
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }
int main()
{
    {
        ObserverList a;
        ObserverList b;
        Check(a.begin() == a.end(), "new list empty");
        Check(b.begin() == b.end() && !(a.end() == b.end()), "separate sentinels");
        a.push_back((NUISystem::CObserver*)0);
        Check(!(a.begin() == a.end()) && *a.begin() == 0, "usable: one element");
        Check(b.begin() == b.end(), "the other list untouched");
    }
    printf(g_fails ? "DLIST_INITIALIZE_EMPTY_FAIL\n" : "DLIST_INITIALIZE_EMPTY_PASS\n");
    return g_fails ? 1 : 0;
}
