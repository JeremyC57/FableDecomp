// CTavernGameCardBaseDef::Copy(const CDefClassBase* other) -- copy every member of another CTavernGameCardBaseDef: `*this = *(const CTavernGameCardBaseDef*)other;`
// Leaf: ?Copy@CTavernGameCardBaseDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EBC11, 5 bytes: a thiscall tail call, `jmp` to CTavernGameCardBaseDef::operator= 0x004EBC16.
// Identity (NF425, 2026-11-25): self ?Copy@CTavernGameCardBaseDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CTavernGameCardBaseDef@@QAEAAV0@ABV0@@Z: bound at 0x004EBC16 (owner Leaf); Ego_r/FableWin name set; its body opens with a call
//   of the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CTavernGameCardBaseDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: the class's own header engine/CTavernGameCardBaseDef.h with the prelude of
//   00/4e/CTavernGameCardBaseDef_GetSizeofClass_004e11bd.cpp (the header declares Copy and operator=).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#include "engine/CTavernGameCardBaseDef.h"

void CTavernGameCardBaseDef::Copy(const CDefClassBase* other)
{
    *this = *(const CTavernGameCardBaseDef*)other;
}
