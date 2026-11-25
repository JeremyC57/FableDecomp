// COpinionOfHeroDef::Copy(const CDefClassBase* other) -- copy every member of another COpinionOfHeroDef: `*this = *(const COpinionOfHeroDef*)other;`
// Leaf: ?Copy@COpinionOfHeroDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0667, 5 bytes: a thiscall tail call, `jmp` to COpinionOfHeroDef::operator= 0x004E066C.
// Identity (NF425, 2026-11-25): self ?Copy@COpinionOfHeroDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4COpinionOfHeroDef@@QAEAAV0@ABV0@@Z: the RTTI-proven Copy of COpinionOfHeroDef tail-jumps to 0x004E066C (its next body; owner is
//   the address-named stand-in ??4CBase_00431f10_Derived_004e066c@@QAEAAU0@ABU0@@Z); Ego_r/FableWin name set; its body opens with a
//   call of the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _COpinionOfHeroDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/COpinionOfHeroDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class COpinionOfHeroDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    COpinionOfHeroDef& operator=(const COpinionOfHeroDef& other);
};

void COpinionOfHeroDef::Copy(const CDefClassBase* other)
{
    *this = *(const COpinionOfHeroDef*)other;
}
