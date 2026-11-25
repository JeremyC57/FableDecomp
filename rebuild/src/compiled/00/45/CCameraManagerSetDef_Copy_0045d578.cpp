// CCameraManagerSetDef::Copy(const CDefClassBase* other) -- copy every member of another CCameraManagerSetDef: `*this = *(const CCameraManagerSetDef*)other;`
// Leaf: ?Copy@CCameraManagerSetDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045D578, 5 bytes: a thiscall tail call, `jmp` to CCameraManagerSetDef::operator= 0x0045D57D.
// Identity (NF425, 2026-11-25): self ?Copy@CCameraManagerSetDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CCameraManagerSetDef@@QAEAAV0@ABV0@@Z: the RTTI-proven Copy of CCameraManagerSetDef tail-jumps to 0x0045D57D (its next body;
//   owner is the address-named stand-in ??4CBase_00431ed0_Derived_0045d57d@@QAEAAU0@ABU0@@Z); Ego_r/FableWin name set; its body opens
//   with a call of the ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CCameraManagerSetDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCameraManagerSetDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a
//   local class over its RTTI direct base CParentDefClassBase engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CCameraManagerSetDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CCameraManagerSetDef& operator=(const CCameraManagerSetDef& other);
};

void CCameraManagerSetDef::Copy(const CDefClassBase* other)
{
    *this = *(const CCameraManagerSetDef*)other;
}
