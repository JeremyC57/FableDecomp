// CFishingRodDef::Copy(const CDefClassBase* other) -- copy every member of another CFishingRodDef: `*this = *(const CFishingRodDef*)other;`
// Leaf: ?Copy@CFishingRodDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E159B, 5 bytes: a thiscall tail call, `jmp` to CFishingRodDef::operator= 0x004E15A0.
// Identity (NF425, 2026-11-25): self ?Copy@CFishingRodDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + FableWin; target
//   ??4CFishingRodDef@@QAEAAV0@ABV0@@Z: the RTTI-proven Copy of CFishingRodDef tail-jumps to 0x004E15A0 (its next body; owner is the
//   address-named stand-in ??4CBase_00431f10_Derived_004e15a0@@QAEAAU0@ABU0@@Z); FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CFishingRodDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CFishingRodDef; a local class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CFishingRodDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CFishingRodDef& operator=(const CFishingRodDef& other);
};

void CFishingRodDef::Copy(const CDefClassBase* other)
{
    *this = *(const CFishingRodDef*)other;
}
