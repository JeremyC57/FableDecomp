// NUISystem::CObservable::ClearObservers() -- empties the observer list Observers (+0x04), drops the exclusive observer (+0x08) and empties ConcurrentExclusiveObservers (+0x0C)
// Leaf: ?ClearObservers@CObservable@NUISystem@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055C950, length 98 bytes (padded to 112).
// Label correction: the catalog "CGraphicBankManager::ProcessLoading" is propagated; the body is slot 6 of the
// NUISystem::CObservable vtable 0x01230044 (rtti-vtables.tsv) among the other CObservable slots (0x0055C930..0x0055CB10).
// Name (NF227, 2026-11-22, q12575 / q12582 / q12583): Ego_r.pdb (0x005D5D60) / FableWin.pdb (0x0225BFA0) / Ego_d.pdb
//   ?ClearObservers@CObservable@NUISystem@@UAEXXZ (public virtual); engine/NUISystem_CObservable.h slot 6, and slot 6 of
//   NUISystem::CManager's vtable 0x01230134 (the address's only two references: .rdata 0x0123005C / 0x0123014C). Was the descriptive
//   ?RemoveAllObservers@CObservable@NUISystem@@QAEXXZ on a row-local CObservable model (m_observers / m_observerCount /
//   m_pendingObservers); the +0x08 word it zeroes is the header's m_pExclusiveObserver.
// Port: onto engine/NUISystem_CObservable.h (Observers +0x04, m_pExclusiveObserver +0x08, ConcurrentExclusiveObservers +0x0C).
// Callees: free @ 0x00BFEA14 (each list node, the STLport allocator's deallocate).
// Scaffolding (row tail): `#pragma optimize("s", on)` after the function, so the list<CObserver*> _List_base::clear COMDAT VC7.1
//   instantiates at the end of the TU takes the size mode of its owner 0x0042A1E3 (that row sets `#pragma optimize("s",on)`; EQ with
//   it, relocations alike); the function itself is compiled at /O2 before it (the plain form grades the same, with that copy 47 v 47 NEQ).
// Shape: two inlined STLport `list::clear()` loops over trivially destructible elements (`tmp = cur; cur = cur->next;
// free(tmp)`, the sentinel member re-read after each free), each followed by the sentinel self-links; the exclusive-observer
// reset sits between the two clears.
#include "engine/NUISystem_CObservable.h"
namespace NUISystem {
void CObservable::ClearObservers()
{
    Observers.clear();
    m_pExclusiveObserver = 0;
    ConcurrentExclusiveObservers.clear();
}
}
// Fidelity: scaffolding, not retail -- the list<CObserver*> clear COMDAT VC7.1 instantiates at the end of the TU takes the owner's size mode
#pragma optimize("s", on)
