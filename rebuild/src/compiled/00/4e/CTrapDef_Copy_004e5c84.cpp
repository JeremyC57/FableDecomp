// CTrapDef::Copy(const CDefClassBase* other) -- copy every member of another CTrapDef: `*this = *(const CTrapDef*)other;`
// Leaf: ?Copy@CTrapDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E5C84, 5 bytes: a thiscall tail call, `jmp` to CTrapDef::operator= 0x004E5C89.
// Identity (NF425, 2026-11-25): self ?Copy@CTrapDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CTrapDef@@QAEAAV0@ABV0@@Z: bound at 0x004E5C89 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the ancestor
//   operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CTrapDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CTrapDef; a local class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CTrapDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CTrapDef& operator=(const CTrapDef& other);
};

void CTrapDef::Copy(const CDefClassBase* other)
{
    *this = *(const CTrapDef*)other;
}
