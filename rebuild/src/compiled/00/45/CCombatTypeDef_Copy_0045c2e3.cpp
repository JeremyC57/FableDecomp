// CCombatTypeDef::Copy(const CDefClassBase* other) -- copy every member of another CCombatTypeDef: `*this = *(const CCombatTypeDef*)other;`
// Leaf: ?Copy@CCombatTypeDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045C2E3, 5 bytes: a thiscall tail call, `jmp` to CCombatTypeDef::operator= 0x0045C2E8.
// Identity (NF425, 2026-11-25): self ?Copy@CCombatTypeDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CCombatTypeDef@@QAEAAV0@ABV0@@Z: bound at 0x0045C2E8 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CCombatTypeDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCombatTypeDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CParentDefClassBase engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CCombatTypeDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CCombatTypeDef& operator=(const CCombatTypeDef& other);
};

void CCombatTypeDef::Copy(const CDefClassBase* other)
{
    *this = *(const CCombatTypeDef*)other;
}
