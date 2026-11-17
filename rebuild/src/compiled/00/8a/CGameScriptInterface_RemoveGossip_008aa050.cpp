// CGameScriptInterface::RemoveGossip(CCharString gossipName) -- remove the gossip entry gossipName from CTCAIScratchpad's static Gossip map
// Leaf: ?RemoveGossip@CGameScriptInterface@@UAEXVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AA050, 50 bytes, thiscall, `ret 4` (the by-value CCharString destroyed here); vtable 0x01260F0C
//   slot 898.
// agentNF57 (q10430): Ego_r.pdb 0x008E90A0 / FableWin.pdb 0x02AC53D0 ?RemoveGossip@CGameScriptInterface@@UAEXVCCharString@@@Z; the body is
//   CTCAIScratchpad::RemoveGossip (FableWin ?RemoveGossip@CTCAIScratchpad@@SIXVCCharString@@@Z) inline: the key copy, then the out-of-line
//   map<CCharString, vector<CCharString> >::erase(const key&) 0x008AE420 on CTCAIScratchpad::Gossip 0x013BAE44 (the ClearGossip 0x008AA010
//   include set, plus the STLport erase-by-key bodies FABLE_RETAIL_STLP_TREE_ERASE_KEY). Was a free __fastcall stand-in over a local
//   Gossip g_gossipObj / GString model.
// Callees: CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0, map::erase(const key&) 0x008AE420.
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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::RemoveGossip(CCharString gossipName)
{
    CCharString localKey(gossipName);
    CTCAIScratchpad::Gossip.erase(localKey);
}
