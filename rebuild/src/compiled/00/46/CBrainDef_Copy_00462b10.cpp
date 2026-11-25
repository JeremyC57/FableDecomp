// CBrainDef::Copy(const CDefClassBase* other) -- copy every member of another CBrainDef: `*this = *(const CBrainDef*)other;`
// Leaf: ?Copy@CBrainDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00462B10, 5 bytes: a thiscall tail call, `jmp` to CBrainDef::operator= 0x00462B15.
// Identity (NF425, 2026-11-25): self ?Copy@CBrainDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CBrainDef@@QAEAAV0@ABV0@@Z: bound at 0x00462B15 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self ?CBrainDef_Copy@@YIXPAVCBrainDef@@PAX@Z; now the real virtual member, so the RTTI-placed name
//   is defined.
// Header: engine header engine/CBrainDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local class
//   over its RTTI direct base CParentDefClassBase engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CBrainDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CBrainDef& operator=(const CBrainDef& other);
};

void CBrainDef::Copy(const CDefClassBase* other)
{
    *this = *(const CBrainDef*)other;
}
