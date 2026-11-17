// CGameScriptInterface::RemoveGossipFactions(CCharString faction) -- remove the faction entry from CTCAIScratchpad's static GossipFactions map
// Leaf: ?RemoveGossipFactions@CGameScriptInterface@@UAEXVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AA510, 50 bytes, thiscall, `ret 4` (the by-value CCharString destroyed here); vtable 0x01260F0C slot 914.
// Identity (NF59, queue q10549): Ego_r.pdb 0x008E9580 / FableWin.pdb 0x02AC73B0 ?RemoveGossipFactions@CGameScriptInterface@@UAEXVCCharString@@@Z;
//   the body is CTCAIScratchpad::RemoveGossipFactions (FableWin 0x02AC7430 ?RemoveGossipFactions@CTCAIScratchpad@@SIXVCCharString@@@Z) inline:
//   the key copy, then map<CCharString, vector<CCharString> >::erase(const key&) 0x008AE420 on CTCAIScratchpad::GossipFactions 0x013BAE5C.
//   Was a free __fastcall stand-in (CGameScriptInterface_RemoveGossipFactions) over local GStr / GGlobal models (g_gossipMgr).
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

void CGameScriptInterface::RemoveGossipFactions(CCharString category)
{
    CCharString localKey(category);
    CTCAIScratchpad::GossipFactions.erase(localKey);
}
