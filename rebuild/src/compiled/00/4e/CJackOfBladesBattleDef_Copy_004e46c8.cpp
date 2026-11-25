// CJackOfBladesBattleDef::Copy(const CDefClassBase* other) -- copy every member of another CJackOfBladesBattleDef: `*this = *(const CJackOfBladesBattleDef*)other;`
// Leaf: ?Copy@CJackOfBladesBattleDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E46C8, 5 bytes: a thiscall tail call, `jmp` to CJackOfBladesBattleDef::operator= 0x004E46CD.
// Identity (NF425, 2026-11-25): self ?Copy@CJackOfBladesBattleDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CJackOfBladesBattleDef@@QAEAAV0@ABV0@@Z: bound at 0x004E46CD (owner Leaf); Ego_r/FableWin name set; its body opens with a call
//   of the ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CJackOfBladesBattleDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CJackOfBladesBattleDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a
//   local class over its RTTI direct base CParentDefClassBase engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CJackOfBladesBattleDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CJackOfBladesBattleDef& operator=(const CJackOfBladesBattleDef& other);
};

void CJackOfBladesBattleDef::Copy(const CDefClassBase* other)
{
    *this = *(const CJackOfBladesBattleDef*)other;
}
