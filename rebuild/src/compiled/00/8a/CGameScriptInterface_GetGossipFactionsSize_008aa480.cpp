// CGameScriptInterface::GetGossipFactionsSize(CCharString name) -- the number of entries in the gossip factions list stored under `name`
// Leaf: ?GetGossipFactionsSize@CGameScriptInterface@@UAEHVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AA480, 66 bytes (thiscall, `ret 4`; no direct callers: CGameScriptInterface vtable slot 912).
// Identity: Ego_r.pdb + FableWin.pdb ?GetGossipFactionsSize@CGameScriptInterface@@UAEHVCCharString@@@Z (engine/CGameScriptInterface.h slot 912); its body is the inline
//   static ?GetGossipFactionsSize@CTCAIScratchpad@@SIHVCCharString@@@Z (FableWin.pdb): the by-value name copied into the static's own by-value
//   key, then CTCAIScratchpad::GossipFactions (0x013BAE5C)[key].size().
// Callees: CCharString(const CCharString&) 0x0099EC30, map<CCharString, vector<CCharString> >::operator[] 0x007AEBE0 (NF85, STLport
//   spelling), ~CCharString 0x0099EAE0 (x2).
// NF87: moved off the row-local models (a free __fastcall stand-in / a Dinkumware <map> of CArray<CLipSyncEntry>) onto the shared headers
//   (CTCAIScratchpad.h GOSSIP_MAPS, NF48's ClearGossip* recipe), so 0x007AEBE0 binds its STLport spelling.
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_MAPS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCAIScratchpad.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

int CGameScriptInterface::GetGossipFactionsSize(CCharString name)
{
    CCharString key(name);
    return CTCAIScratchpad::GossipFactions[key].size();
}
