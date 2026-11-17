// CGameScriptInterface::GetGossipSize(CCharString gossipName) -- the number of gossip lines held under gossipName in CTCAIScratchpad's
//   static Gossip map (the entry is created when absent)
// Leaf: ?GetGossipSize@CGameScriptInterface@@UAEHVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A9FC0, 64 bytes (+ padding), thiscall, `ret 4` (the by-value CCharString destroyed here);
//   vtable 0x01260F0C slot 896.
// agentNF57 (q10430): Ego_r.pdb 0x008E9010 / FableWin.pdb 0x02AC4F10 ?GetGossipSize@CGameScriptInterface@@UAEHVCCharString@@@Z; the map at
//   0x013BAE44 is CTCAIScratchpad::Gossip (the ClearGossip 0x008AA010 include set: engine/CTCAIScratchpad.h FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_MAPS,
//   operator[] 0x007AEBE0 out of line). Was the free __fastcall stand-in CGameScriptInterface_GetGossipSize over a local GossipMap
//   g_gossipVillagesMap (the map is Gossip, not GossipVillages 0x013BAE50).
// Callees: CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0, map<CCharString, vector<CCharString> >::operator[] 0x007AEBE0.
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

int CGameScriptInterface::GetGossipSize(CCharString gossipName)
{
    CCharString localKey(gossipName);
    return CTCAIScratchpad::Gossip[localKey].size();
}
