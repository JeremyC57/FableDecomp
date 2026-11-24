// Behaviour test for 0x0055c9c0 (NF230, q12624) on the shared header engine/NUISystem_CObservable.h: NUISystem::CObservable::RemoveObserver, the row's
// body under the row's defines, on the header class (a test-only subclass reads the protected lists; a test observer counts
// IgnoreAllEvents). One case per condition in the row. Stubbed: the other CObservable virtuals (the vtable of the implicit
// constructor needs them) and CObserver's out-of-line constructor / destructor / virtuals. Replaces the HEAD host model.
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_INPUT
#include <stdio.h>
#include "engine/NUISystem_CObservable.h"
#include "stlport/fable_algorithm.h"
namespace NUISystem {
void CObservable::RemoveObserver(CObserver* observer)
{
    std::list<CObserver*>::iterator it = std::find(Observers.begin(), Observers.end(), observer);
    if (it != Observers.end()) {
        (*it)->IgnoreAllEvents();
        Observers.erase(it);
    }
}
CObserver::CObserver() { }
CObserver::~CObserver() { }
bool CObserver::IsObservingEvent(EEvent) { return false; }
void CObserver::ObserveEvent(EEvent) { }
void CObserver::IgnoreEvent(EEvent) { }
void CObserver::IgnoreAllEvents() { }
void CObserver::ObserveAllEvents() { }
void CObserver::SetPreventObserving(bool) { }
void CObservable::SetEvent(NUISystem::EEvent) { }
void CObservable::SetOwnedEvent(NUISystem::CObserver*, NUISystem::EEvent) { }
void CObservable::AddObserver(NUISystem::CObserver*) { }
void CObservable::AddExclusiveObserver(NUISystem::CObserver*) { }
void CObservable::RemoveExclusiveObserver(NUISystem::CObserver*) { }
void CObservable::ClearObservers() { }
NUISystem::CObserver* CObservable::GetExclusiveObserver() { return 0; }
bool CObservable::IsExclusivelyObserved() { return false; }
void CObservable::AddConcurrentExclusiveObserver(NUISystem::CObserver*) { }
void CObservable::RemoveConcurrentExclusiveObserver(NUISystem::CObserver*) { }
}
struct CTestObserver : NUISystem::CObserver {
    int Ignored;
    CTestObserver() : Ignored(0) { }
    void ProcessEvent(NUISystem::EEvent) { }
    void IgnoreAllEvents() { ++Ignored; }
};
struct CTestObservable : NUISystem::CObservable {
    std::list<NUISystem::CObserver*>& Obs() { return Observers; }
    std::list<NUISystem::CObserver*>& Conc() { return ConcurrentExclusiveObservers; }
    NUISystem::CObserver*& Excl() { return m_pExclusiveObserver; }
    CTestObservable() { m_pExclusiveObserver = 0; }
};
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }
int main()
{
    {
        CTestObservable p; CTestObserver x, y, z;
        p.Obs().push_back(&x); p.Obs().push_back(&y); p.Obs().push_back(&z);
        p.RemoveObserver(&y);
        Check(p.Obs().size() == 2 && p.Obs().front() == &x && p.Obs().back() == &z, "a present observer is erased, the others keep their order");
        Check(y.Ignored == 1 && x.Ignored == 0 && z.Ignored == 0, "the erased observer ignores all events, once");
        CTestObserver w;
        p.RemoveObserver(&w);
        Check(p.Obs().size() == 2 && w.Ignored == 0, "an absent observer: nothing erased, nothing told");
        p.Conc().push_back(&x);
        p.RemoveObserver(&x);
        Check(p.Obs().size() == 1 && p.Conc().size() == 1, "only the Observers list is searched");
    }
    printf(g_fails ? "0055c9c0_TEST FAIL\n" : "0055c9c0_TEST PASS\n");
    return g_fails ? 1 : 0;
}
