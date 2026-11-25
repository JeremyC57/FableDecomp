// CHeroMorphDef::Copy(const CDefClassBase* other) -- copy every member of another CHeroMorphDef: `*this = *(const CHeroMorphDef*)other;`
// Leaf: ?Copy@CHeroMorphDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E6F06, 5 bytes: a thiscall tail call, `jmp` to CHeroMorphDef::operator= 0x004E6F0B.
// Identity (NF425, 2026-11-25): self ?Copy@CHeroMorphDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CHeroMorphDef@@QAEAAV0@ABV0@@Z: bound at 0x004E6F0B (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CHeroMorphDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CHeroMorphDef; a local class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CHeroMorphDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CHeroMorphDef& operator=(const CHeroMorphDef& other);
};

void CHeroMorphDef::Copy(const CDefClassBase* other)
{
    *this = *(const CHeroMorphDef*)other;
}
