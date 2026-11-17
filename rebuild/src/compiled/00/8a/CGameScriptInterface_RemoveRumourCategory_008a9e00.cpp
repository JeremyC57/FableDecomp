// CGameScriptInterface::RemoveRumourCategory(const CCharString& category) const -- remove the rumour category from CTCAIScratchpad's static Gossip map
// Leaf: ?RemoveRumourCategory@CGameScriptInterface@@UBEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A9E00, 41 bytes, thiscall, `ret 4`; vtable 0x01260F0C slot 719.
// Identity (NF59, queue q10549): Ego_r.pdb 0x008E8E30 / FableWin.pdb 0x02AAFAF0 ?RemoveRumourCategory@CGameScriptInterface@@UBEXABVCCharString@@@Z
//   (engine/CGameScriptInterface.h slot 719); the body is the key copy, then map<CCharString, vector<CCharString> >::erase(const key&)
//   0x008AE420 on CTCAIScratchpad::Gossip 0x013BAE44, as the landed RemoveGossip 0x008AA050 (NF57). Was a free __fastcall stand-in
//   (CGameScriptInterface_RemoveRumourCategory) over local Cat / Mgr models and the cast address 0x13BAE44.
// Callees: CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0, map::erase(const key&) 0x008AE420.
// Model: NF57's 0x008AA050 include set (shared STLport containers / map with FABLE_RETAIL_STLP_TREE_ERASE_KEY, the retail CTCAIScratchpad
//   gossip maps, the retail CGameScriptInterface).
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

void CGameScriptInterface::RemoveRumourCategory(const CCharString& category) const
{
    CCharString localKey(category);
    CTCAIScratchpad::Gossip.erase(localKey);
}
