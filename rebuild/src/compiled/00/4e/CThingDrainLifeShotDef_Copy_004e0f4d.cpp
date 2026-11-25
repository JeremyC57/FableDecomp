// CThingDrainLifeShotDef::Copy(const CDefClassBase* other) -- copy every member of another CThingDrainLifeShotDef: `*this = *(const CThingDrainLifeShotDef*)other;`
// Leaf: ?Copy@CThingDrainLifeShotDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0F4D, 5 bytes: a thiscall tail call, `jmp` to CThingDrainLifeShotDef::operator= 0x004E0F52.
// Identity (NF425, 2026-11-25): self ?Copy@CThingDrainLifeShotDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CThingDrainLifeShotDef@@QAEAAV0@ABV0@@Z: the RTTI-proven Copy of CThingDrainLifeShotDef tail-jumps to 0x004E0F52 (its next body;
//   owner is the address-named stand-in ??4CBase_00431f10_Derived_004e0f52@@QAEAAU0@ABU0@@Z); Ego_r/FableWin name set; its body opens
//   with a call of the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CThingDrainLifeShotDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CThingDrainLifeShotDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a
//   local class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CThingDrainLifeShotDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CThingDrainLifeShotDef& operator=(const CThingDrainLifeShotDef& other);
};

void CThingDrainLifeShotDef::Copy(const CDefClassBase* other)
{
    *this = *(const CThingDrainLifeShotDef*)other;
}
