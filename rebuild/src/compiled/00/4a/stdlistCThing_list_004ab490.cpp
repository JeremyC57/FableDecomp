// std::_List_base<CThing*>::~_List_base() -- STLport list base destructor: clear() (free every node after the header, the header
//   pointer re-read after each free, relink the header to itself), then free the header
// Leaf: ??1?$_List_base@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AB490, 62 bytes, thiscall `ret`.
// Name fix (NF239, 2026-11-22, NF237 follow-up / nf237 ruling): the STLport 4.6 _List_base destructor (the clear + free body lives in
//   _List_base, _list.h; list's own `~list() {}` is empty), the spelling the landed binders 0x004A3740 (CWorld::UpdateRegionLoad) and
//   0x0079A890 (CTCSummonSpell::IsValidTeleport...) already bind here. Was ??1?$list@PAVCThing@@..@QAE@XZ from a row-local std::list
//   model; ~CTCCreatureItemLevitationBase 0x004DE6ED moves with it (it called the local ~list), and 0x004FE030's ??1CThingList@@ stand-in
//   is left (NOTES). Identity (sweep3; FINDINGS chunk 478): ~CTCCreatureItemLevitationBase 0x004DE6ED calls it on this+0x0C (Ego_r.pdb
//   LevitatingItems list<CThing*>); other callers CWorld::LoadGameState, CWorldMap::CompileLoadableGlobalThingsToSingleFile 0x004FE1F3,
//   CTCSummonSpell 0x0079AA69, CTCAssassinRush 0x007AA9B7; the element is a plain pointer (no element destructor in the loop).
// Model: stlport/fable_containers.h (the shared STLport 4.6 _List_base: clear inline, deallocate's null test); the explicit
//   instantiation emits the destructor as its own COMDAT. Callee: free @ 0x00BFEA14.
#include "stlport/fable_containers.h"
class CThing;
template std::_List_base<CThing*, std::allocator<CThing*> >::~_List_base();
