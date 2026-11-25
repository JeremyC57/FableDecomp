// CThunderBattleDef::Copy(const CDefClassBase* other) -- copy every member of another CThunderBattleDef: `*this = *(const CThunderBattleDef*)other;`
// Leaf: ?Copy@CThunderBattleDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E45F2, 5 bytes: a thiscall tail call, `jmp` to CThunderBattleDef::operator= 0x004E45F7.
// Identity (NF425, 2026-11-25): self ?Copy@CThunderBattleDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CThunderBattleDef@@QAEAAV0@ABV0@@Z: the RTTI-proven Copy of CThunderBattleDef tail-jumps to 0x004E45F7 (its next body; owner is
//   the address-named stand-in ??4CBase_00431ed0_Derived_004e45f7@@QAEAAU0@ABU0@@Z); Ego_r/FableWin name set; its body opens with a
//   call of the ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CThunderBattleDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: the class's own header engine/CThunderBattleDef.h with the prelude of 00/4e/CThunderBattleDef_GetSizeofClass_004e38e0.cpp
//   (the header declares Copy and operator=).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CThunderBattleDef.h"

void CThunderBattleDef::Copy(const CDefClassBase* other)
{
    *this = *(const CThunderBattleDef*)other;
}
