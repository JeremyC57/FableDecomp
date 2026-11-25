// CLightningOrbDef::Copy(const CDefClassBase* other) -- copy every member of another CLightningOrbDef: `*this = *(const CLightningOrbDef*)other;`
// Leaf: ?Copy@CLightningOrbDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E1A21, 5 bytes: a thiscall tail call, `jmp` to CLightningOrbDef::operator= 0x004E1A26.
// Identity (NF425, 2026-11-25): self ?Copy@CLightningOrbDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CLightningOrbDef@@QAEAAV0@ABV0@@Z: the RTTI-proven Copy of CLightningOrbDef tail-jumps to 0x004E1A26 (its next body; owner is
//   the address-named stand-in ??4CBase_00431f10_Derived_004e1a26@@QAEAAU0@ABU0@@Z); Ego_r/FableWin name set; its body opens with a
//   call of the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CLightningOrbDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CLightningOrbDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CLightningOrbDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CLightningOrbDef& operator=(const CLightningOrbDef& other);
};

void CLightningOrbDef::Copy(const CDefClassBase* other)
{
    *this = *(const CLightningOrbDef*)other;
}
