// CGameScriptInterface::RemoveGossipVillages(CCharString village) -- remove the village entry from CTCAIScratchpad's static GossipVillages map
// Leaf: ?RemoveGossipVillages@CGameScriptInterface@@UAEXVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AA2B0, 50 bytes, thiscall, `ret 4` (the by-value CCharString destroyed here); vtable 0x01260F0C slot 906.
// Identity (NF59, queue q10549): Ego_r.pdb 0x008E9310 / FableWin.pdb 0x02AC6300 ?RemoveGossipVillages@CGameScriptInterface@@UAEXVCCharString@@@Z;
//   the body is CTCAIScratchpad::RemoveGossipVillages (FableWin 0x02AC6380 ?RemoveGossipVillages@CTCAIScratchpad@@SIXVCCharString@@@Z) inline:
//   the key copy, then map<CCharString, vector<CCharString> >::erase(const key&) 0x008AE420 on CTCAIScratchpad::GossipVillages 0x013BAE50.
//   Was a free __fastcall stand-in (CGameScriptInterface_RemoveGossipVillages) over local Str / GObj models and the cast address 0x13BAE50.
// Callees: CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0, map::erase(const key&) 0x008AE420.
// Model: NF57's 0x008AA050 include set.
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_MAPS
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_STLP_TREE_ERASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCAIScratchpad.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::RemoveGossipVillages(CCharString category)
{
    CCharString localKey(category);
    CTCAIScratchpad::GossipVillages.erase(localKey);
}
