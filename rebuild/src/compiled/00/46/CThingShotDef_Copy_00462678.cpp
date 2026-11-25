// CThingShotDef::Copy(const CDefClassBase* other) -- copy every member of another CThingShotDef: `*this = *(const CThingShotDef*)other;`
// Leaf: ?Copy@CThingShotDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00462678, 5 bytes: a thiscall tail call, `jmp` to CThingShotDef::operator= 0x0046267D.
// Identity (NF425, 2026-11-25): self ?Copy@CThingShotDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CThingShotDef@@QAEAAV0@ABV0@@Z: bound at 0x0046267D (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CThingShotDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CThingShotDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CParentDefClassBase engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CThingShotDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CThingShotDef& operator=(const CThingShotDef& other);
};

void CThingShotDef::Copy(const CDefClassBase* other)
{
    *this = *(const CThingShotDef*)other;
}
