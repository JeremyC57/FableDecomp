// CGameScriptInterface::ClearGossipFactions(CCharString gossipName) -- empty the gossip list held under gossipName in CTCAIScratchpad's static
//   GossipFactions map (the entry is created when absent)
// Leaf: ?ClearGossipFactions@CGameScriptInterface@@UAEXVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AA4D0, 64 bytes, thiscall, `ret 4` (the by-value CCharString destroyed here); vtable 0x01260F0C
//   slot 913.
// NF48 (round 7): the DAT_013bae5c GossipFactionLipSyncMap / CArray::EraseRange stand-ins and the free __fastcall CGameScriptInterface_ClearGossipFactions(CGameScriptInterface*, CCharString) stand-in are gone (the definition now carries the Ego_r.pdb member signature). The map is Ego_r.pdb's static
//   CTCAIScratchpad::GossipFactions (0x013BAE5C; `dynamic initializer for 'CTCAIScratchpad::GossipFactions'`), an STLport 4.6.2
//   map<CCharString, vector<CCharString> > (the value type CGameScriptInterface::GetGossipFactions 0x008AA410 returns); engine/CTCAIScratchpad.h
//   declares it under FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_MAPS (overlay opt-in). operator[] is the out-of-line
//   map<CCharString, vector<CCharString> >::operator[] 0x007AEBE0 (stlport/fable_map.h declaration); clear() is the header's inline
//   erase(begin(), end()) over the out-of-line vector<CCharString>::erase(iterator, iterator) 0x0043336A
//   (FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE).
// Callees: CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0 (the key copy and the argument).
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_MAPS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCAIScratchpad.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::ClearGossipFactions(CCharString gossipName)
{
    CCharString localKey(gossipName);
    CTCAIScratchpad::GossipFactions[localKey].clear();
}
