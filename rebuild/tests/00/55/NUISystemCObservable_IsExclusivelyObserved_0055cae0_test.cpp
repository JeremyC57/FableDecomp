// Behaviour test for 0x0055cae0 (NF230, q12624) on the shared header engine/NUISystem_CObservable.h: NUISystem::CObservable::IsExclusivelyObserved, the row's
// body under the row's defines, on the header class (a test-only subclass reads the protected lists; a test observer counts
// IgnoreAllEvents). One case per condition in the row. Stubbed: the other CObservable virtuals (the vtable of the implicit
// constructor needs them) and CObserver's out-of-line constructor / destructor / virtuals. Replaces the HEAD host model.
#define FABLE_RETAIL_STLP_LIST
#include <stdio.h>
#include "engine/NUISystem_CObservable.h"
namespace NUISystem {
bool CObservable::IsExclusivelyObserved()
{
    if (m_pExclusiveObserver)
        return true;
    if (ConcurrentExclusiveObservers.size() != 0)
        return true;
    return false;
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
void CObservable::RemoveObserver(NUISystem::CObserver*) { }
void CObservable::ClearObservers() { }
NUISystem::CObserver* CObservable::GetExclusiveObserver() { return 0; }
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
        CTestObservable p; CTestObserver x;
        Check(!p.IsExclusivelyObserved(), "no exclusive and no concurrent exclusive observer: false");
        p.Excl() = &x;
        Check(p.IsExclusivelyObserved(), "an exclusive observer: true");
        p.Conc().push_back(&x);
        Check(p.IsExclusivelyObserved(), "both: true");
        p.Excl() = 0;
        Check(p.IsExclusivelyObserved(), "a concurrent exclusive observer only: true");
        p.Conc().clear(); p.Obs().push_back(&x);
        Check(!p.IsExclusivelyObserved(), "ordinary observers do not count");
    }
    printf(g_fails ? "0055cae0_TEST FAIL\n" : "0055cae0_TEST PASS\n");
    return g_fails ? 1 : 0;
}
