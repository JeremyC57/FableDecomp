// Behaviour test for 0x0042BE7B NUISystem::CObservable::CObservable() on the shared header engine/NUISystem_CObservable.h:
// a new observable has no exclusive observer and two empty observer lists (each list's sentinel node linked to itself).
// The constructor body is the row's; the eleven virtuals the vtable references are stubbed (only GetExclusiveObserver /
// IsExclusivelyObserved carry their retail bodies, 0x0042BEA9 / 0x0055CAE0, for the checks).
#define FABLE_RETAIL_COBSERVABLE_CTOR_DECL_ONLY
#define FABLE_RETAIL_STLP_LIST
#include <stdio.h>
#include "engine/NUISystem_CObservable.h"
namespace NUISystem {
CObservable::CObservable()
    : m_pExclusiveObserver(0)
{
}
void CObservable::SetEvent(EEvent) { }
void CObservable::SetOwnedEvent(CObserver*, EEvent) { }
void CObservable::AddObserver(CObserver*) { }
void CObservable::AddExclusiveObserver(CObserver*) { }
void CObservable::RemoveExclusiveObserver(CObserver*) { }
void CObservable::RemoveObserver(CObserver*) { }
void CObservable::ClearObservers() { }
CObserver* CObservable::GetExclusiveObserver() { return m_pExclusiveObserver; }
bool CObservable::IsExclusivelyObserved() { return m_pExclusiveObserver != 0; }
void CObservable::AddConcurrentExclusiveObserver(CObserver*) { }
void CObservable::RemoveConcurrentExclusiveObserver(CObserver*) { }
}
struct CObservableProbe : NUISystem::CObservable {
    bool ObserversEmpty() { return Observers.begin() == Observers.end(); }
    bool ConcurrentEmpty() { return ConcurrentExclusiveObservers.begin() == ConcurrentExclusiveObservers.end(); }
};
int main()
{
    int fails = 0;
    {
        CObservableProbe o;
        if (o.GetExclusiveObserver() != 0) { printf("case 1 FAIL: exclusive observer not cleared\n"); ++fails; }
        if (o.IsExclusivelyObserved()) { printf("case 2 FAIL: reports an exclusive observer\n"); ++fails; }
        if (!o.ObserversEmpty()) { printf("case 3 FAIL: Observers not empty\n"); ++fails; }
        if (!o.ConcurrentEmpty()) { printf("case 4 FAIL: ConcurrentExclusiveObservers not empty\n"); ++fails; }
        if (*(void**)&o == 0) { printf("case 5 FAIL: no vtable pointer\n"); ++fails; }
    }
    {
        CObservableProbe a, b;   // two objects: independent sentinel nodes
        if (a.ObserversEmpty() != true || b.ObserversEmpty() != true) { printf("case 6 FAIL\n"); ++fails; }
    }
    printf(fails ? "0042be7b_TEST FAIL\n" : "0042be7b_TEST PASS\n");
    return fails ? 1 : 0;
}
