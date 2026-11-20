// CCharString::operator!=(const CCharString& other) const -- negation of the inline string equality (same block, or same length and equal characters)
// Leaf: ??9CCharString@@QBE_NABV0@@Z
// Retail: Fable.exe @ 0x0099E900, 83 bytes, `ret 4`; ~30 callers (0x00499CBB, 0x004AF7D1, ... 0x009987EF); Ego_r.pdb
// ??9CCharString@@QBE_NABV0@@Z. Catalog row label "std::less<CCharString>::operator()" is wrong (the ghidra label file
// already says CCharString::operator!=, chunk 227): every exit negates the equality result (`test al,al ; sete cl`).
// Body: return !(*this == other), operator== inlined: same data block -> true; both blocks non-null with equal length
// (+4) -> Compare(chars, other chars) == 0 (0x00411570, __fastcall); else false.
// Layout: CCharString {CCharStringData* PStringData}; data block {const char* chars +0, length +4} (the landed 0x0083CC10
// model; Ego_r.pdb CCharStringData = CBasicString<char> Data + NoRefs).
// Shape: /O2; the equality returns `Compare(...) == 0` (`neg ; sbb ; inc`).
// Name fix (NF199, 2026-11-20, q4451): the local CCharString / CCharStringData model and its free `Compare` stand-in for 0x00411570
//   are replaced by the shared fable_string.h (FABLE_RETAIL_CCHARSTRING: CCharStringData { CBasicString<char> Data; NoRefs }); the
//   inline CCharString::operator== -> CBasicString<char>::operator== (length, then Compare) now binds the protected static
//   ?Compare@?$CBasicString@D@@KIJPBD0@Z (Ego_r.pdb), the self row 0x00411570's Leaf.
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
bool CCharString::operator!=(const CCharString& other) const
{
    return !(*this == other);
}
