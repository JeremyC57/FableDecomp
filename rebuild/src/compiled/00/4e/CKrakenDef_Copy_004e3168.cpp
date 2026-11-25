// CKrakenDef::Copy(const CDefClassBase* other) -- copy every member of another CKrakenDef: `*this = *(const CKrakenDef*)other;`
// Leaf: ?Copy@CKrakenDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E3168, 5 bytes: a thiscall tail call, `jmp` to CKrakenDef::operator= 0x004E316D.
// Identity (NF425, 2026-11-25): self ?Copy@CKrakenDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CKrakenDef@@QAEAAV0@ABV0@@Z: bound at 0x004E316D (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CKrakenDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: the class's own header engine/CKrakenDef.h with the prelude of 00/4d/CKrakenDef_GetSizeofClass_004defa3.cpp (the header
//   declares Copy and operator=).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CKrakenDef.h"

void CKrakenDef::Copy(const CDefClassBase* other)
{
    *this = *(const CKrakenDef*)other;
}
