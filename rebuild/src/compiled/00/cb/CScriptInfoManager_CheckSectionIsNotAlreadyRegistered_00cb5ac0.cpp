// CScriptInfoManager::CheckSectionIsNotAlreadyRegistered(CCharString section) -- empty in retail: the callee only destroys its by-value argument
// (`lea ecx,[esp+4] ; call CCharString::~CCharString ; ret 4`)
// Leaf: ?CheckSectionIsNotAlreadyRegistered@CScriptInfoManager@@IAEXVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB5AC0, 12 bytes, thiscall, `ret 4`. Callee: CCharString::~CCharString 0x0099EAE0.
// NF39 (2026-11-16): moved off its `Allocator::Cons_val_pair_morph_i(PairSlot*)` stand-in onto the shared engine/CScriptInfoManager.h (which names
//   0x00CB5AC0): Ego_r.pdb + FableWin.pdb name sets ?CheckSectionIsNotAlreadyRegistered@CScriptInfoManager@@IAEXVCCharString@@@Z (protected); donor
//   order puts it directly before GetPScriptInfo 0x00CB5AD0 in both; RegisterScripts 0x00CD52D0 calls it with `this` in ecx and a CCharString copy.
//   Presumably a debug-build check compiled out of the release build (only the by-value parameter's destruction remains).
// Defines: the landed 0x00CD52D0 RegisterScripts preamble.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTINFO_CTOR
#include <stddef.h>
#include "fable_string.h"
#include "engine/CScriptInfo.h"
#include "engine/CScriptInfoManager.h"

void CScriptInfoManager::CheckSectionIsNotAlreadyRegistered(CCharString)
{
}
