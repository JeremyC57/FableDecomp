// CThingBaseDef::Copy(const CDefClassBase* other) -- copy every member of another CThingBaseDef: `*this = *(const CThingBaseDef*)other;`
// Leaf: ?Copy@CThingBaseDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00677390, 5 bytes: a thiscall tail call, `jmp` to CThingBaseDef::operator= 0x00463950.
// Identity (NF425, 2026-11-25): self ?Copy@CThingBaseDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CThingBaseDef@@QAEAAV0@ABV0@@Z: bound at 0x00463950 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CThingBaseDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CThingBaseDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CParentDefClassBase engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CThingBaseDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CThingBaseDef& operator=(const CThingBaseDef& other);
};

void CThingBaseDef::Copy(const CDefClassBase* other)
{
    *this = *(const CThingBaseDef*)other;
}
