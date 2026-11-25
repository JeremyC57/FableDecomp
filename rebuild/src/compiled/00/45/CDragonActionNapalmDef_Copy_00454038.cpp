// CDragonActionNapalmDef::Copy(const CDefClassBase* other) -- copy every member of another CDragonActionNapalmDef: `*this = *(const CDragonActionNapalmDef*)other;`
// Leaf: ?Copy@CDragonActionNapalmDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00454038, 5 bytes: a thiscall tail call, `jmp` to CDragonActionNapalmDef::operator= 0x0045403D.
// Identity (NF425, 2026-11-25): self ?Copy@CDragonActionNapalmDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + FableWin; target
//   ??4CDragonActionNapalmDef@@QAEAAV0@ABV0@@Z: the RTTI-proven Copy of CDragonActionNapalmDef tail-jumps to 0x0045403D (its next body;
//   owner is the address-named stand-in ??4CBase_00431f10_Derived_0045403d@@QAEAAU0@ABU0@@Z); FableWin name set; its body opens with a
//   call of the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self ?CDragonActionNapalmDef_Copy@@YIXPAVCDragonActionNapalmDef@@PAX@Z; now the real virtual
//   member, so the RTTI-placed name is defined.
// Header: no engine header defines CDragonActionNapalmDef; a local class over its RTTI direct base CSubDefClassBase
//   engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CDragonActionNapalmDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CDragonActionNapalmDef& operator=(const CDragonActionNapalmDef& other);
};

void CDragonActionNapalmDef::Copy(const CDefClassBase* other)
{
    *this = *(const CDragonActionNapalmDef*)other;
}
