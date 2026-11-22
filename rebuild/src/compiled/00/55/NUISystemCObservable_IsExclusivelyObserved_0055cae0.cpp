// NUISystem::CObservable::IsExclusivelyObserved() -- true when an exclusive observer is set or any concurrent exclusive observer is listed
// Leaf: ?IsExclusivelyObserved@CObservable@NUISystem@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055CAE0, 35 bytes (plain `ret`), no callees.
// Label: CObservable vtable 0x01230044 slot 8 and NUISystem::CManager's vtable 0x01230134 slot 8 (its only two references);
// Ego_r.pdb / FableWin.pdb / Ego_d.pdb ?IsExclusivelyObserved@CObservable@NUISystem@@UAE_NXZ (public virtual).
// Port (NF227, 2026-11-22, q12582 / q12583): onto engine/NUISystem_CObservable.h (m_pExclusiveObserver +0x08,
//   ConcurrentExclusiveObservers +0x0C, an STLport list). Was a row-local CObservable model with a non-virtual member, leaf
//   ?IsExclusivelyObserved@CObservable@NUISystem@@QAE_NXZ, while the header vtable COMDAT of 0x0041E641 / 0x0041E3F6 binds the slot as UAE.
//   FABLE_RETAIL_STLP_LIST gives STLport 4.6 list::size() (the distance loop).
// Shape: STLport `list::size()` walks the nodes (`distance` into a zeroed counter); two sequential `if (..) return true;` tests share the `mov al,1`
// tail placed before the `xor al,al` one (`if (a || b) return true;` and `if (!a && !b) return false;` put the false tail
// first, `return a || b;` materialises `mov eax,1` / `xor eax,eax`).
#define FABLE_RETAIL_STLP_LIST
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
}
