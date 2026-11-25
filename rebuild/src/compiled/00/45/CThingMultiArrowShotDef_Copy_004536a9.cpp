// CThingMultiArrowShotDef::Copy(const CDefClassBase* other) -- copy every member of another CThingMultiArrowShotDef: `*this = *(const CThingMultiArrowShotDef*)other;`
// Leaf: ?Copy@CThingMultiArrowShotDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004536A9, 5 bytes: a thiscall tail call, `jmp` to CThingMultiArrowShotDef::operator= 0x004536AE.
// Identity (NF425, 2026-11-25): self ?Copy@CThingMultiArrowShotDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CThingMultiArrowShotDef@@QAEAAV0@ABV0@@Z: bound at 0x004536AE (owner Leaf); Ego_r/FableWin name set; its body opens with a call
//   of the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self ?CThingMultiArrowShotDef_Copy@@YIXPAVCThingMultiArrowShotDef@@PAXPBV1@@Z; now the real virtual
//   member, so the RTTI-placed name is defined.
// Header: engine header engine/CThingMultiArrowShotDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a
//   local class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CThingMultiArrowShotDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CThingMultiArrowShotDef& operator=(const CThingMultiArrowShotDef& other);
};

void CThingMultiArrowShotDef::Copy(const CDefClassBase* other)
{
    *this = *(const CThingMultiArrowShotDef*)other;
}
