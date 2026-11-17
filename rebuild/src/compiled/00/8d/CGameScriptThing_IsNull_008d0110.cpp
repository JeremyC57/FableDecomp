// CGameScriptThing::IsNull() const -- true when the script thing's name is the null-thing name "%%%NULL"
// Leaf: ?IsNull@CGameScriptThing@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D0110, 51 bytes: Name (+0x20) against the CCharString at 0x013BAFCC -- same data block,
//   either null, StringLength (+4) differs, else `call 0x00411570 ; neg eax ; sbb al,al ; inc al`.
// NF52 (round 7): moved onto the shared headers. The row was a free __fastcall stand-in CGameScriptThing_IsNull over a local
// CCharStringData model, the made-up extern "C" Thing_Compare and a pointer-typed extern g_nullThing.
// Identity: Ego_r.pdb 0x00907140 / FableWin.pdb 0x02B007A0 ?IsNull@CGameScriptThing@@UBE_NXZ, the override of CScriptThing slot 77
//   (engine/CScriptThing.h IsNull 0x004AB150), declared by the overlay opt-in FABLE_RETAIL_CGAMESCRIPTTHING_ISNULL; reached through the
//   vtable only (no E8 caller).
// Body: the inline CCharString::operator==(const CCharString&) of fable_string.h (data pointers, null tests, then
//   CBasicString<char>::operator== = StringLength test and !Compare), which binds the protected static
//   ?Compare@?$CBasicString@D@@KIJPBD0@Z (0x00411570).
// Data: 0x013BAFCC is the CCharString("%%%NULL") the landed initializer 0x0121F740 defines as g_CCharString_013bafcc (its spelling,
//   one address one name; Ego_r.pdb's initializer list has an anonymous-namespace SThingNullName, a candidate, see NAME_FIXES.tsv).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CGAMESCRIPTTHING
#define FABLE_RETAIL_CGAMESCRIPTTHING_ISNULL
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CGameScriptThing.h"

extern CCharString g_CCharString_013bafcc;              // 0x013BAFCC ("%%%NULL", landed initializer 0x0121F740)

bool CGameScriptThing::IsNull() const
{
    return Name == g_CCharString_013bafcc;
}
