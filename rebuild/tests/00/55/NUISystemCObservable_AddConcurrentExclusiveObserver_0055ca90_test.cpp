// Behaviour test for 0x0055ca90 (NF230, q12624) on the shared header engine/NUISystem_CObservable.h: NUISystem::CObservable::AddConcurrentExclusiveObserver, the row's
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
void CObservable::AddConcurrentExclusiveObserver(CObserver* observer)
{
    std::list<CObserver*>::iterator it = std::find(ConcurrentExclusiveObservers.begin(), ConcurrentExclusiveObservers.end(), observer);
    if (it == ConcurrentExclusiveObservers.end())
        ConcurrentExclusiveObservers.push_back(observer);
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
bool CObservable::IsExclusivelyObserved() { return false; }
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
        CTestObservable p; CTestObserver x, y;
        p.AddConcurrentExclusiveObserver(&x);
        Check(p.Conc().size() == 1 && p.Conc().front() == &x, "first observer added to an empty list");
        p.AddConcurrentExclusiveObserver(&y);
        Check(p.Conc().size() == 2 && p.Conc().back() == &y, "a new observer is appended");
        p.AddConcurrentExclusiveObserver(&x);
        Check(p.Conc().size() == 2 && p.Conc().front() == &x && p.Conc().back() == &y, "a present observer is not added twice");
        Check(p.Obs().size() == 0 && p.Excl() == 0, "the Observers list and the exclusive observer untouched");
    }
    printf(g_fails ? "0055ca90_TEST FAIL\n" : "0055ca90_TEST PASS\n");
    return g_fails ? 1 : 0;
}
