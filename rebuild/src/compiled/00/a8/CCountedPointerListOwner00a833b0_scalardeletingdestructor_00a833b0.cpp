// CCountedPointerListOwner_00a833b0::`scalar deleting destructor'(unsigned int) -- clear the counted-pointer list at +4, free its sentinel node, then free the object when bit 0 of the flag is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A833B0, length 52 bytes.
// Callees: the list's clear @ 0x00A80A20 (landed as CRetailListCountedPointer::clear; releases each
// node's counted pointer), free @ 0x00BFEA14 (the sentinel node, explicit null test),
// operator delete @ 0x00BFE9BC. The manifest label "CTCAssassinRush::FinishMove" is propagated; no
// call or vtable references the body, so the class is named by what it owns.
// Shape: no vtable store and `test byte ptr [esp+0Ch],1` = the NON-virtual `??_G` that VC7.1 emits
// for a plain class deleted with `delete` (Wave 24m): the implicit destructor is inlined, the 4-byte
// STLport list at +4 runs its inline destructor (clear(), then free the node read back through the
// list's address), the word at +0 is trivially destroyed.
// Leaf: ??_G
// NF50 (round 7): the local CCountedPointerList stand-in is the shared stlport/fable_containers.h list<CCountedPointer<CNavRegionConnection> >
// (nf41 e175: 0x00A80A20 is its _List_base::clear, Ego_r.pdb CNavRegion::Connections; FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE, declaration
// only), so the clear call binds ?clear@?$_List_base@V?$CCountedPointer@VCNavRegionConnection@@@@V?$allocator@V?$CCountedPointer@VCNavRegionConnection@@@@@std@@@std@@QAEXXZ.
// The owner keeps its descriptive stand-in name (NF50 NOTES).
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "stlport/fable_containers.h"
class CNavRegionConnection;
#include "engine/CCountedPointer.h"

struct CCountedPointerListOwner_00a833b0 {
    long                m_key;                              // +0x00
    std::list<CCountedPointer<CNavRegionConnection> > m_list;   // +0x04
};

void DeleteCountedPointerListOwner(CCountedPointerListOwner_00a833b0* owner)
{
    delete owner;
}