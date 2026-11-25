// CEnvironmentThemeDef::Copy(const CDefClassBase* other) -- copy every member of another CEnvironmentThemeDef: `*this = *(const CEnvironmentThemeDef*)other;`
// Leaf: ?Copy@CEnvironmentThemeDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004734B2, 5 bytes: a thiscall tail call, `jmp` to CEnvironmentThemeDef::operator= 0x00473046.
// Identity (NF425, 2026-11-25): self ?Copy@CEnvironmentThemeDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CEnvironmentThemeDef@@QAEAAV0@ABV0@@Z: bound at 0x00473046 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of
//   the ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CEnvironmentThemeDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CEnvironmentThemeDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a
//   local class over its RTTI direct base CParentDefClassBase engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CEnvironmentThemeDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CEnvironmentThemeDef& operator=(const CEnvironmentThemeDef& other);
};

void CEnvironmentThemeDef::Copy(const CDefClassBase* other)
{
    *this = *(const CEnvironmentThemeDef*)other;
}
