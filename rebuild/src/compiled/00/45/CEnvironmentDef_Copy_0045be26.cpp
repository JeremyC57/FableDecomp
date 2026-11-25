// CEnvironmentDef::Copy(const CDefClassBase* other) -- copy every member of another CEnvironmentDef: `*this = *(const CEnvironmentDef*)other;`
// Leaf: ?Copy@CEnvironmentDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045BE26, 5 bytes: a thiscall tail call, `jmp` to CEnvironmentDef::operator= 0x0045BE2B.
// Identity (NF425, 2026-11-25): self ?Copy@CEnvironmentDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CEnvironmentDef@@QAEAAV0@ABV0@@Z: bound at 0x0045BE2B (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CEnvironmentDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CEnvironmentDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CParentDefClassBase engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CEnvironmentDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CEnvironmentDef& operator=(const CEnvironmentDef& other);
};

void CEnvironmentDef::Copy(const CDefClassBase* other)
{
    *this = *(const CEnvironmentDef*)other;
}
