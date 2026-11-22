// NUISystem::CObservable::RemoveObserver(CObserver* observer) -- unregister an observer: find it, make it ignore all events, unlink it
// Leaf: ?RemoveObserver@CObservable@NUISystem@@UAEXPAVCObserver@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055C9C0, 63 bytes (`ret 4`).
// Callees: the observer's virtual slot 5 (+0x14, IgnoreAllEvents(), 0x0052D9A0 in CObserver's vtable), free @ 0x00BFEA14.
// Label: CObservable vtable 0x01230044 slot 5 (rtti-vtables.tsv); Ego_r.pdb names
// ?RemoveObserver@CObservable@NUISystem@@UAEXPAVCObserver@2@@Z. Slot 10 (0x0055CA00) is the same body on the concurrent
// exclusive list (RemoveConcurrentExclusiveObserver).
// Layout: as the landed CObservable constructor 0x0042BE7B / ClearObservers 0x0055C950: Observers +0x04 (STLport list, the
// sentinel pointer), m_pExclusiveObserver +0x08, ConcurrentExclusiveObservers +0x0c.
// Shape: STLport `std::find` inlined over list iterators (the value in edx, `mov edi,edi` loop alignment), `end()` re-read
// for the test, then `erase(it)`: unlink through next/prev, free the node.
// Port (NF227, 2026-11-22, q12582 / q12583): onto engine/NUISystem_CObservable.h (Observers +0x04, m_pExclusiveObserver +0x08,
//   ConcurrentExclusiveObservers +0x0C, STLport lists; CObserver from engine/NUISystem_CComponent.h). Was a row-local CObservable /
//   CObserver / std::list model with a non-virtual member, leaf ?RemoveObserver@CObservable@NUISystem@@QAEXPAVCObserver@2@@Z, while the header vtable
//   COMDAT of 0x0041E641 / 0x0041E3F6 binds the slot as UAE (Ego_r.pdb / FableWin.pdb / Ego_d.pdb spell it UAE, public virtual).
//   The STLport input-iterator find (FABLE_RETAIL_STLP_FIND_INPUT, with FABLE_RETAIL_STLP_FIND_IF_INPUT's list iterator traits) is
//   expanded in place; FABLE_RETAIL_STLP_LIST gives list::erase.
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_INPUT
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
}
