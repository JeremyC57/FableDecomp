// CFishDef::Copy(const CDefClassBase* other) -- copy every member of another CFishDef: `*this = *(const CFishDef*)other;`
// Leaf: ?Copy@CFishDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E12B3, 5 bytes: a thiscall tail call, `jmp` to CFishDef::operator= 0x004E12B8.
// Identity (NF425, 2026-11-25): self ?Copy@CFishDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CFishDef@@QAEAAV0@ABV0@@Z: bound at 0x004E12B8 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the ancestor
//   operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CFishDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CFishDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local class
//   over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CFishDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CFishDef& operator=(const CFishDef& other);
};

void CFishDef::Copy(const CDefClassBase* other)
{
    *this = *(const CFishDef*)other;
}
