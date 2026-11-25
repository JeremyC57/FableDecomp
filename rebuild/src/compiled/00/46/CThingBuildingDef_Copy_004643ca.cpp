// CThingBuildingDef::Copy(const CDefClassBase* other) -- copy every member of another CThingBuildingDef: `*this = *(const CThingBuildingDef*)other;`
// Leaf: ?Copy@CThingBuildingDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004643CA, 5 bytes: a thiscall tail call, `jmp` to CThingBuildingDef::operator= 0x004643CF.
// Identity (NF425, 2026-11-25): self ?Copy@CThingBuildingDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CThingBuildingDef@@QAEAAV0@ABV0@@Z: bound at 0x004643CF (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CThingBaseDef (0x00463950): a derived class's assignment.
// Respell (NF425): was the stand-in self _CThingBuildingDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CThingBuildingDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CThingBaseDef engine/CThingBaseDef.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CThingBaseDef.h"

class CThingBuildingDef : public CThingBaseDef
{
public:
    virtual void Copy(const CDefClassBase* other);
    CThingBuildingDef& operator=(const CThingBuildingDef& other);
};

void CThingBuildingDef::Copy(const CDefClassBase* other)
{
    *this = *(const CThingBuildingDef*)other;
}
