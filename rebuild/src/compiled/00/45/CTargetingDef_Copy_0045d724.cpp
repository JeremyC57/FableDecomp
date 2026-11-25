// CTargetingDef::Copy(const CDefClassBase* other) -- copy every member of another CTargetingDef: `*this = *(const CTargetingDef*)other;`
// Leaf: ?Copy@CTargetingDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045D724, 5 bytes: a thiscall tail call, `jmp` to CTargetingDef::operator= 0x0045D729.
// Identity (NF425, 2026-11-25): self ?Copy@CTargetingDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CTargetingDef@@QAEAAV0@ABV0@@Z: bound at 0x0045D729 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CTargetingDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CTargetingDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CTargetingDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CTargetingDef& operator=(const CTargetingDef& other);
};

void CTargetingDef::Copy(const CDefClassBase* other)
{
    *this = *(const CTargetingDef*)other;
}
