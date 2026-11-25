// CTurncoatDef::Copy(const CDefClassBase* other) -- copy every member of another CTurncoatDef: `*this = *(const CTurncoatDef*)other;`
// Leaf: ?Copy@CTurncoatDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E2CDF, 5 bytes: a thiscall tail call, `jmp` to CTurncoatDef::operator= 0x004E2CE4.
// Identity (NF425, 2026-11-25): self ?Copy@CTurncoatDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CTurncoatDef@@QAEAAV0@ABV0@@Z: bound at 0x004E2CE4 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CTurncoatDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CTurncoatDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CTurncoatDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CTurncoatDef& operator=(const CTurncoatDef& other);
};

void CTurncoatDef::Copy(const CDefClassBase* other)
{
    *this = *(const CTurncoatDef*)other;
}
