// CGameScriptInterface::SetActiveGossipCategories(CCharString category, bool active) -- set whether the named gossip category is active (by-value form)
// Leaf: ?SetActiveGossipCategories@CGameScriptInterface@@UAEXVCCharString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A9720, thiscall, `ret 8` (no direct callers: CGameScriptInterface vtable slot 884).
// Identity: Ego_r.pdb + FableWin.pdb ?SetActiveGossipCategories@CGameScriptInterface@@UAEXVCCharString@@_N@Z (engine/CGameScriptInterface.h slot 884); its body is the inline static
//   ?SetActiveGossipCategories@CTCAIScratchpad@@SIXVCCharString@@_N@Z (FableWin.pdb): the name copied into the static's by-value key, then
//   CTCAIScratchpad::ActiveGossipCategories (0x013BAE2C)[key] = active.
// Callees: CCharString(const CCharString&) 0x0099EC30, map<CCharString, bool>::operator[] 0x008ADF10 (NF85, STLport spelling), ~CCharString
//   0x0099EAE0.
// NF87: moved off the row-local Key / Map stand-ins onto CGameScriptInterface.h + CTCAIScratchpad.h (overlay opt-in
//   FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_BOOL_MAPS), so 0x008ADF10 binds its STLport spelling.
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_BOOL_MAPS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCAIScratchpad.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SetActiveGossipCategories(CCharString category, bool active)
{
    CCharString key(category);
    CTCAIScratchpad::ActiveGossipCategories[key] = active;
}
