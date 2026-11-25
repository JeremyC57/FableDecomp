// CTCVolumeContainmentTrackerDef::Copy(const CDefClassBase* other) -- copy every member of another CTCVolumeContainmentTrackerDef: `*this = *(const CTCVolumeContainmentTrackerDef*)other;`
// Leaf: ?Copy@CTCVolumeContainmentTrackerDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E1897, 5 bytes: a thiscall tail call, `jmp` to CTCVolumeContainmentTrackerDef::operator= 0x004E189C.
// Identity (NF425, 2026-11-25): self ?Copy@CTCVolumeContainmentTrackerDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin;
//   target ??4CTCVolumeContainmentTrackerDef@@QAEAAV0@ABV0@@Z: the RTTI-proven Copy of CTCVolumeContainmentTrackerDef tail-jumps to
//   0x004E189C (its next body; owner is the address-named stand-in ??4CBase_00431f10_Derived_004e189c@@QAEAAU0@ABU0@@Z); Ego_r/FableWin
//   name set; its body opens with a call of the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CTCVolumeContainmentTrackerDef_Copy; now the real virtual member, so the RTTI-placed name is
//   defined.
// Header: engine header engine/CTCVolumeContainmentTrackerDef.h does not declare Copy / operator= in any landed prelude tried (header
//   debt); a local class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CTCVolumeContainmentTrackerDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CTCVolumeContainmentTrackerDef& operator=(const CTCVolumeContainmentTrackerDef& other);
};

void CTCVolumeContainmentTrackerDef::Copy(const CDefClassBase* other)
{
    *this = *(const CTCVolumeContainmentTrackerDef*)other;
}
