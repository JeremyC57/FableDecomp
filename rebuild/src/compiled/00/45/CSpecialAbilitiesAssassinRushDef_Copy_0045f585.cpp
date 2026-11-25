// CSpecialAbilitiesAssassinRushDef::Copy(const CDefClassBase* other) -- copy every member of another CSpecialAbilitiesAssassinRushDef: `*this = *(const CSpecialAbilitiesAssassinRushDef*)other;`
// Leaf: ?Copy@CSpecialAbilitiesAssassinRushDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045F585, 5 bytes: a thiscall tail call, `jmp` to CSpecialAbilitiesAssassinRushDef::operator= 0x0045F58A.
// Identity (NF425, 2026-11-25): self ?Copy@CSpecialAbilitiesAssassinRushDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin;
//   target ??4CSpecialAbilitiesAssassinRushDef@@QAEAAV0@ABV0@@Z: bound at 0x0045F58A (owner Leaf); Ego_r/FableWin name set; its body
//   opens with a call of the ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CSpecialAbilitiesAssassinRushDef_Copy; now the real virtual member, so the RTTI-placed name
//   is defined.
// Header: no engine header defines CSpecialAbilitiesAssassinRushDef; a local class over its RTTI direct base CParentDefClassBase
//   engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CSpecialAbilitiesAssassinRushDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CSpecialAbilitiesAssassinRushDef& operator=(const CSpecialAbilitiesAssassinRushDef& other);
};

void CSpecialAbilitiesAssassinRushDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesAssassinRushDef*)other;
}
