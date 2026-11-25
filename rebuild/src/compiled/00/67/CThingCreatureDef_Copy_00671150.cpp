// CThingCreatureDef::Copy(const CDefClassBase* other) -- copy every member of another CThingCreatureDef: `*this = *(const CThingCreatureDef*)other;`
// Leaf: ?Copy@CThingCreatureDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00671150, 5 bytes: a thiscall tail call, `jmp` to CThingCreatureDef::operator= 0x00671160.
// Identity (NF425, 2026-11-25): self ?Copy@CThingCreatureDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CThingCreatureDef@@QAEAAV0@ABV0@@Z: bound at 0x00671160 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CThingBaseDef (0x00463950): a derived class's assignment.
// Respell (NF425): was the stand-in self ?Copy@CControlsDef@@QAEXPBV1@@Z; now the real virtual member, so the RTTI-placed name is
//   defined.
// Header: engine header engine/CThingCreatureDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CThingBaseDef engine/CThingBaseDef.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CThingBaseDef.h"

class CThingCreatureDef : public CThingBaseDef
{
public:
    virtual void Copy(const CDefClassBase* other);
    CThingCreatureDef& operator=(const CThingCreatureDef& other);
};

void CThingCreatureDef::Copy(const CDefClassBase* other)
{
    *this = *(const CThingCreatureDef*)other;
}
