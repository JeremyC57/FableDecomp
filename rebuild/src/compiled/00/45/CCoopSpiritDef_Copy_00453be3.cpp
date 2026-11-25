// CCoopSpiritDef::Copy(const CDefClassBase* other) -- copy every member of another CCoopSpiritDef: `*this = *(const CCoopSpiritDef*)other;`
// Leaf: ?Copy@CCoopSpiritDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453BE3, 5 bytes: a thiscall tail call, `jmp` to CCoopSpiritDef::operator= 0x00453BE8.
// Identity (NF425, 2026-11-25): self ?Copy@CCoopSpiritDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CCoopSpiritDef@@QAEAAV0@ABV0@@Z: the RTTI-proven Copy of CCoopSpiritDef tail-jumps to 0x00453BE8 (its next body; owner is the
//   address-named stand-in ??4CBase_00431f10_Derived_00453be8@@QAEAAU0@ABU0@@Z); Ego_r/FableWin name set; its body opens with a call of
//   the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self ?CCoopSpiritDef_Copy@@YIXPAVCCoopSpiritDef@@PBV1@@Z; now the real virtual member, so the
//   RTTI-placed name is defined.
// Header: no engine header defines CCoopSpiritDef; a local class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CCoopSpiritDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CCoopSpiritDef& operator=(const CCoopSpiritDef& other);
};

void CCoopSpiritDef::Copy(const CDefClassBase* other)
{
    *this = *(const CCoopSpiritDef*)other;
}
