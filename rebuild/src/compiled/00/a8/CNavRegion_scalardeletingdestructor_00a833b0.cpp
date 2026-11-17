// CNavRegion::`scalar deleting destructor'(unsigned int) -- clear the Connections list at +4, free its sentinel node, then free the object when bit 0 of the flag is set
// Leaf: ??_GCNavRegion@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A833B0, length 52 bytes.
// Callees: the list's clear @ 0x00A80A20 (_List_base<CCountedPointer<CNavRegionConnection> >::clear; releases each node's counted
// pointer), free @ 0x00BFEA14 (the sentinel node, explicit null test), operator delete @ 0x00BFE9BC. The manifest label
// "CTCAssassinRush::FinishMove" is propagated; no call, jump or data reference reaches the body (an unreferenced COMDAT).
// Shape: no vtable store and `test byte ptr [esp+0Ch],1` = the NON-virtual `??_G` that VC7.1 emits for a plain class deleted with
// `delete` (Wave 24m): the implicit destructor is inlined, the 4-byte STLport list at +4 runs its inline destructor (clear(), then
// free the node read back through the list's address), the word at +0 is trivially destroyed.
// Identity (NF58, round 7): the owner is CNavRegion (engine/CNavRegion.h). Two sources: (1) the body destroys exactly a
//   list<CCountedPointer<CNavRegionConnection> > at +4 (its clear 0x00A80A20 is that instance's, nf41 e175) after a trivially destroyed
//   word at +0; (2) Ego_r.pdb's only class with a member of that type is CNavRegion (RegionID long +0, Connections +4; MostRecentSearchID
//   +8 is trivial too). Was the stand-in CCountedPointerListOwner_00a833b0 (a local model of the same layout); the stand-in is gone and
//   the row deletes a CNavRegion through the shared header. Descriptive by layout (no caller names it): see NF58 NOTES.
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "stlport/fable_containers.h"
#include "engine/CNavRegion.h"
void DeleteNavRegion(CNavRegion* region)
{
    delete region;
}
