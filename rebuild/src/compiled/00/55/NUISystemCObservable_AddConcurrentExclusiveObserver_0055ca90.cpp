// NUISystem::CObservable::AddConcurrentExclusiveObserver(CObserver* observer) -- register a concurrent exclusive observer once: append it unless it is already listed
// Leaf: ?AddConcurrentExclusiveObserver@CObservable@NUISystem@@UAEXPAVCObserver@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055CA90, 69 bytes (`ret 4`).
// Callee: malloc @ 0x00BFEA0E (the list node).
// Label: CObservable vtable 0x01230044 slot 9 (rtti-vtables.tsv); Ego_r.pdb names
// ?AddConcurrentExclusiveObserver@CObservable@NUISystem@@UAEXPAVCObserver@2@@Z. Slot 2 (0x0055CA40, AddObserver) is the
// same body on Observers.
// Layout: as the landed CObservable constructor 0x0042BE7B: Observers +0x04, m_pExclusiveObserver +0x08,
// ConcurrentExclusiveObservers +0x0c (each STLport list the 4-byte sentinel pointer).
// Shape: STLport `std::find` inlined over list iterators (value in edi, `nop` loop alignment) into a NAMED iterator (the
// find written inside the `if` compares with the cached end and jumps past the re-read); when it reaches `end()`
// (re-read into esi) the inlined push_back: node malloc, null-tested placement copy, four link stores before the sentinel.
// Port (NF227, 2026-11-22, q12582 / q12583): onto engine/NUISystem_CObservable.h (Observers +0x04, m_pExclusiveObserver +0x08,
//   ConcurrentExclusiveObservers +0x0C, STLport lists; CObserver from engine/NUISystem_CComponent.h). Was a row-local CObservable /
//   CObserver / std::list model with a non-virtual member, leaf ?AddConcurrentExclusiveObserver@CObservable@NUISystem@@QAEXPAVCObserver@2@@Z, while the header vtable
//   COMDAT of 0x0041E641 / 0x0041E3F6 binds the slot as UAE (Ego_r.pdb / FableWin.pdb / Ego_d.pdb spell it UAE, public virtual).
//   The STLport input-iterator find (FABLE_RETAIL_STLP_FIND_INPUT, with FABLE_RETAIL_STLP_FIND_IF_INPUT's list iterator traits) is
//   expanded in place; FABLE_RETAIL_STLP_LIST gives list::erase.
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_INPUT
#include "engine/NUISystem_CObservable.h"
#include "stlport/fable_algorithm.h"
namespace NUISystem {
void CObservable::AddConcurrentExclusiveObserver(CObserver* observer)
{
    std::list<CObserver*>::iterator it = std::find(ConcurrentExclusiveObservers.begin(), ConcurrentExclusiveObservers.end(), observer);
    if (it == ConcurrentExclusiveObservers.end())
        ConcurrentExclusiveObservers.push_back(observer);
}
}
