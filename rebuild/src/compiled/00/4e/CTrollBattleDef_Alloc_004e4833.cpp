// CTrollBattleDef::Alloc() -- the def factory: new CTrollBattleDef (operator new 0x00BFEA1A, 0x60 bytes), the inline default constructor (the out-of-line CParentDefClassBase() 0x00430370, then the CTrollBattleDef vtable) when the block is not null
// Leaf: ?Alloc@CTrollBattleDef@@SIPAVCDefClassBase@@XZ
#pragma optimize("s",on)
// Retail: Fable.exe @ 0x004E4833 (size-optimised def TU, as the landed row had it). Identity: the vtable it stores, 0x01242F0C, is RTTI
//   .?AVCTrollBattleDef@@ : CParentDefClassBase (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ?Alloc@CTrollBattleDef@@SIPAVCDefClassBase@@XZ.
// Callees: operator new 0x00BFEA1A, CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ).
// Model: the shared engine/CTrollBattleDef.h (hdr overlay opt-in FABLE_RETAIL_CTROLLBATTLEDEF_MEMBERS declares Alloc and the overrides)
//   over engine/CParentDefClassBase.h; the constructor is the implicit one (no members with constructors).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTROLLBATTLEDEF_MEMBERS
#include "fable_string.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CTrollBattleDef.h"

CDefClassBase* FABLE_FASTCALL CTrollBattleDef::Alloc()
{
    return new CTrollBattleDef;
}
