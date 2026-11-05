// Leaf: ?CAbilityButtonGuiDef_Copy@@YIPAVCAbilityButtonGuiDef@@PAV1@PBVCDefClassBase@@@Z
#include "rebuild_abi.h"

// 0x0046f49f  ?Copy@CAbilityButtonGuiDef@@UAEXPBVCDefClassBase@@@Z
// 5-byte tail-call thunk: jmp rel32 to the shared Copy implementation.
// __fastcall accessor modeled as free __fastcall(thisptr, arg). The rel32
// target is relocation-masked, so any plausible extern callee byte-matches.

class CDefClassBase;
class CAbilityButtonGuiDef;

// Shared Copy implementation this thunk tail-calls into.
extern "C" CAbilityButtonGuiDef* __fastcall
CAbilityButtonGuiDef_Copy_impl_0046f49f(CAbilityButtonGuiDef* thisptr, const CDefClassBase* src);

CAbilityButtonGuiDef* __fastcall
CAbilityButtonGuiDef_Copy(CAbilityButtonGuiDef* thisptr, const CDefClassBase* src)
{
    // Tail call -> single `jmp rel32`.
    return CAbilityButtonGuiDef_Copy_impl_0046f49f(thisptr, src);
}
// Stand-in (owner rule 2026-11-05): CAbilityButtonGuiDef_Copy for CAbilityButtonGuiDef::Copy; RTTI vtable slot ?Copy@CAbilityButtonGuiDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
