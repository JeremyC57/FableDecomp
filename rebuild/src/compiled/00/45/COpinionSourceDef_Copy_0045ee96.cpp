// COpinionSourceDef::Copy(const CDefClassBase* other) -- copy every member of another COpinionSourceDef: `*this = *(const COpinionSourceDef*)other;`
// Leaf: ?Copy@COpinionSourceDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045EE96, 5 bytes: a thiscall tail call, `jmp` to COpinionSourceDef::operator= 0x0045EE9B.
// Identity (NF425, 2026-11-25): self ?Copy@COpinionSourceDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4COpinionSourceDef@@QAEAAV0@ABV0@@Z: bound at 0x0045EE9B (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _COpinionSourceDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines COpinionSourceDef; a local class over its RTTI direct base CParentDefClassBase
//   engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class COpinionSourceDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    COpinionSourceDef& operator=(const COpinionSourceDef& other);
};

void COpinionSourceDef::Copy(const CDefClassBase* other)
{
    *this = *(const COpinionSourceDef*)other;
}
