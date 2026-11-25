// CHeroTitleDef::Copy(const CDefClassBase* other) -- copy every member of another CHeroTitleDef: `*this = *(const CHeroTitleDef*)other;`
// Leaf: ?Copy@CHeroTitleDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453CE5, 5 bytes: a thiscall tail call, `jmp` to CHeroTitleDef::operator= 0x00453CEA.
// Identity (NF425, 2026-11-25): self ?Copy@CHeroTitleDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CHeroTitleDef@@QAEAAV0@ABV0@@Z: bound at 0x00453CEA (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self ?CHeroTitleDef_Copy@@YIXPAVCHeroTitleDef@@PAX@Z; now the real virtual member, so the
//   RTTI-placed name is defined.
// Header: engine header engine/CHeroTitleDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CHeroTitleDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CHeroTitleDef& operator=(const CHeroTitleDef& other);
};

void CHeroTitleDef::Copy(const CDefClassBase* other)
{
    *this = *(const CHeroTitleDef*)other;
}
