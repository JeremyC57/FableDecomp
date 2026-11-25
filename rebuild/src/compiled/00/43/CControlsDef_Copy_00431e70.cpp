// CControlsDef::Copy(const CDefClassBase* other) -- copy every member of another CControlsDef: `*this = *(const CControlsDef*)other;`
// Leaf: ?Copy@CControlsDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00431E70, 5 bytes: a thiscall tail call, `jmp` to CControlsDef::operator= 0x00431E80.
// Identity (NF425, 2026-11-25): self ?Copy@CControlsDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CControlsDef@@QAEAAV0@ABV0@@Z: bound at 0x00431E80 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self ?Copy@CControlsDef@@QAEXPBV1@@Z; now the real virtual member, so the RTTI-placed name is
//   defined.
// Header: no engine header defines CControlsDef; a local class over its RTTI direct base CParentDefClassBase
//   engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CControlsDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CControlsDef& operator=(const CControlsDef& other);
};

void CControlsDef::Copy(const CDefClassBase* other)
{
    *this = *(const CControlsDef*)other;
}
