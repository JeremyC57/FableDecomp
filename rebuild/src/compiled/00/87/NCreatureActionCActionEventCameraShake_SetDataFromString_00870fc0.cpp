// NCreatureAction::CActionEventCameraShake::SetDataFromString(const CCharString& data) -- parse "strength length [GLOBAL]" camera-shake event data: field 0 picks the EQuakeStrength, field 1 the EQuakeLength, and a third field "GLOBAL" sets Global
// Leaf: ?SetDataFromString@CActionEventCameraShake@NCreatureAction@@MAE_NABVCCharString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00870FC0, CActionEventCameraShake vtable slot 4 (0x0125C10C); Ego_r.pdb 0x00811990 /
//   FableWin.pdb 0x028371B0 ?SetDataFromString@CActionEventCameraShake@NCreatureAction@@MAE_NABVCCharString@@@Z (protected virtual).
// Callees: `anonymous namespace'::GetDataFromEventString(const CCharString&, long, CCharString&) @ 0x00870C90 (__fastcall),
//   CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0, CBasicString<char>::operator==(const char*) const 0x004115A0.
// Layout: engine/NCreatureActionEvents.h (Ego_r.pdb CActionEventCameraShake: Strength +0x14, Length +0x18, Global +0x1c).
// Shape: each field is a named local in its own block, compared with the inline CCharString::operator==(const char*) (VC7.1
//   shares the one null test across each block's tests); the CBasicString<char>::operator==(const char*) body is visible to the
//   TU and called out of line, so VC7.1 keeps the field's string data in esi across the calls.
// Name fix (NF216, 2026-11-21, q12378): ported onto fable_string.h + engine/NCreatureActionEvents.h. The `#define CCharString
//   CCharStringPdb` rename-include and the local CBasicString / CCharStringData / CCharString / CActionEventBase /
//   CActionEventCameraShake models (with their same-TU signed-char == specialization under auto_inline(off)) are replaced by
//   FABLE_RETAIL_CCHARSTRING + FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ + FABLE_RETAIL_CBASICSTRING_COMPARE_BODY +
//   FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO; the pick-any ??8?$CBasicString@D@@QBE_NPBD@Z copy this object carries
//   is now the owner 0x004115A0's 88-byte body. Self leaf UAE -> the PDB's MAE (protected virtual, both PDBs; no landed row
//   references 0x00870FC0: no E8/E9 caller, its one absolute reference is the vtable 0x0125C10C, in no landed row).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#include <string.h>
#include "fable_string.h"
#include "engine/NCreatureActionEvents.h"

namespace { bool __fastcall GetDataFromEventString(const CCharString& data, long index, CCharString& out); }   // 0x00870C90

namespace NCreatureAction {

bool CActionEventCameraShake::SetDataFromString(const CCharString& data)
{
    {
        CCharString value;
        if (!GetDataFromEventString(data, 0, value)) return false;
        if (value == "STRENGTH_WEAK") Strength = QUAKE_STRENGTH_WEAK;
        else if (value == "STRENGTH_MEDIUM") Strength = QUAKE_STRENGTH_MEDIUM;
        else if (value == "STRENGTH_STRONG") Strength = QUAKE_STRENGTH_STRONG;
        else if (value == "STRENGTH_MADNESS") Strength = QUAKE_STRENGTH_MADNESS;
        else return false;
    }
    {
        CCharString length;
        if (!GetDataFromEventString(data, 1, length)) return false;
        if (length == "LENGTH_SHORT") Length = QUAKE_LENGTH_SHORT;
        else if (length == "LENGTH_MEDIUM") Length = QUAKE_LENGTH_MEDIUM;
        else if (length == "LENGTH_LONG") Length = QUAKE_LENGTH_LONG;
        else if (length == "LENGTH_CONTINUOUS") Length = QUAKE_LENGTH_CONTINUOUS;
        else return false;
    }
    {
        CCharString global;
        if (!GetDataFromEventString(data, 2, global)) return false;
        if (global == "GLOBAL") Global = true;
    }
    return true;
}

}
