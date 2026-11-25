// CRegionDef::Copy(const CDefClassBase* other) -- copy every member of another CRegionDef: `*this = *(const CRegionDef*)other;`
// Leaf: ?Copy@CRegionDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045CE43, 5 bytes: a thiscall tail call, `jmp` to CRegionDef::operator= 0x0045CE48.
// Identity (NF425, 2026-11-25): self ?Copy@CRegionDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CRegionDef@@QAEAAV0@ABV0@@Z: bound at 0x0045CE48 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CRegionDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CRegionDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local class
//   over its RTTI direct base CParentDefClassBase engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CRegionDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CRegionDef& operator=(const CRegionDef& other);
};

void CRegionDef::Copy(const CDefClassBase* other)
{
    *this = *(const CRegionDef*)other;
}
