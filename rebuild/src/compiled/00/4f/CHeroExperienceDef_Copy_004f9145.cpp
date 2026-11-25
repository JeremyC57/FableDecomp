// CHeroExperienceDef::Copy(const CDefClassBase* other) -- copy every member of another CHeroExperienceDef: `*this = *(const CHeroExperienceDef*)other;`
// Leaf: ?Copy@CHeroExperienceDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F9145, 5 bytes: a thiscall tail call, `jmp` to CHeroExperienceDef::operator= 0x004F914A.
// Identity (NF425, 2026-11-25): self ?Copy@CHeroExperienceDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CHeroExperienceDef@@QAEAAV0@ABV0@@Z: bound at 0x004F914A (owner Leaf); Ego_r/FableWin name set; its body opens with a call of
//   the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CHeroExperienceDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CHeroExperienceDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a
//   local class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CHeroExperienceDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CHeroExperienceDef& operator=(const CHeroExperienceDef& other);
};

void CHeroExperienceDef::Copy(const CDefClassBase* other)
{
    *this = *(const CHeroExperienceDef*)other;
}
