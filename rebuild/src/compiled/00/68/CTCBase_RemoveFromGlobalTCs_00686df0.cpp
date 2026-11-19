// CTCBase::RemoveFromGlobalTCs() -- clear IsInGlobalUpdateTCs (+0x09) and remove this component from the global update container.
// Leaf: ?RemoveFromGlobalTCs@CTCBase@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686DF0, thiscall.
// Name (agentNF32, 2026-11-12; the leaf was the free __fastcall ?CTCBase_RemoveFromGlobalTCs@@YIXPAVCTCBase@@@Z): Ego_r.pdb 0x006B21B0 and
//   FableWin.pdb 0x021919D0 ?RemoveFromGlobalTCs@CTCBase@@QAEXXZ (public member, no arguments); engine/CTCBase.h declares it
//   (FABLE_RETAIL_CTCBASE); the landed 0x005C00A0 binds this address under that symbol.
// Names (NF95, 2026-11-19, queue q11245): the container 0x013BA8AC is Ego_d.pdb PUB32 ?GlobalUpdateTCs@CTCBase@@1V?$list@PAVCTCBase@@..@std@@A
//   (protected static list<CTCBase*>; engine/CTCBase.h FABLE_RETAIL_CTCBASE_GLOBALUPDATETCS) and its helper 0x004CC150 is the STLport
//   list<CTCBase*>::remove (?remove@?$list@PAVCTCBase@@V?$allocator@PAVCTCBase@@@std@@@std@@QAEXABQAVCTCBase@@@Z, the landed self row's
//   symbol; 0x004C8050 / 0x004C9D60 bind it so), called out of line (FABLE_RETAIL_STLP_LIST_REMOVE_DECL). Was the stand-in
//   GContainer::Remove on ?g_container@@3UGContainer@@A.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_GLOBALUPDATETCS
#define FABLE_RETAIL_STLP_LIST_REMOVE_DECL
#include "stlport/fable_containers.h"
#include "engine/CTCBase.h"

void CTCBase::RemoveFromGlobalTCs()
{
    IsInGlobalUpdateTCs = false;
    GlobalUpdateTCs.remove(this);
}
