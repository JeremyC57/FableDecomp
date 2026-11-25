// CAreaOfEffectAttackDef::Copy(const CDefClassBase* other) -- copy every member of another CAreaOfEffectAttackDef: `*this = *(const CAreaOfEffectAttackDef*)other;`
// Leaf: ?Copy@CAreaOfEffectAttackDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E6CC1, 5 bytes: a thiscall tail call, `jmp` to CAreaOfEffectAttackDef::operator= 0x004E6CC6.
// Identity (NF425, 2026-11-25): self ?Copy@CAreaOfEffectAttackDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CAreaOfEffectAttackDef@@QAEAAV0@ABV0@@Z: the RTTI-proven Copy of CAreaOfEffectAttackDef tail-jumps to 0x004E6CC6 (its next body;
//   owner is the address-named stand-in ??4CBase_00431ed0_Derived_004e6cc6@@QAEAAU0@ABU0@@Z); Ego_r/FableWin name set; its body opens
//   with a call of the ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CAreaOfEffectAttackDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CAreaOfEffectAttackDef; a local class over its RTTI direct base CParentDefClassBase
//   engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CAreaOfEffectAttackDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CAreaOfEffectAttackDef& operator=(const CAreaOfEffectAttackDef& other);
};

void CAreaOfEffectAttackDef::Copy(const CDefClassBase* other)
{
    *this = *(const CAreaOfEffectAttackDef*)other;
}
