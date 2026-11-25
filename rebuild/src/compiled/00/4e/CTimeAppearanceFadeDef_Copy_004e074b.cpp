// CTimeAppearanceFadeDef::Copy(const CDefClassBase* other) -- copy every member of another CTimeAppearanceFadeDef: `*this = *(const CTimeAppearanceFadeDef*)other;`
// Leaf: ?Copy@CTimeAppearanceFadeDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E074B, 5 bytes: a thiscall tail call, `jmp` to CTimeAppearanceFadeDef::operator= 0x004E0750.
// Identity (NF425, 2026-11-25): self ?Copy@CTimeAppearanceFadeDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + FableWin; target
//   ??4CTimeAppearanceFadeDef@@QAEAAV0@ABV0@@Z: the RTTI-proven Copy of CTimeAppearanceFadeDef tail-jumps to 0x004E0750 (its next body;
//   owner is the address-named stand-in ??4CBase_00431f10_Derived_004e0750@@QAEAAU0@ABU0@@Z); FableWin name set; its body opens with a
//   call of the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CTimeAppearanceFadeDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CTimeAppearanceFadeDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a
//   local class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CTimeAppearanceFadeDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CTimeAppearanceFadeDef& operator=(const CTimeAppearanceFadeDef& other);
};

void CTimeAppearanceFadeDef::Copy(const CDefClassBase* other)
{
    *this = *(const CTimeAppearanceFadeDef*)other;
}
