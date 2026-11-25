// CBedDef::Copy(const CDefClassBase* other) -- copy every member of another CBedDef: `*this = *(const CBedDef*)other;`
// Leaf: ?Copy@CBedDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DFFBE, 5 bytes: a thiscall tail call, `jmp` to CBedDef::operator= 0x004DFFC3.
// Identity (NF425, 2026-11-25): self ?Copy@CBedDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CBedDef@@QAEAAV0@ABV0@@Z: the RTTI-proven Copy of CBedDef tail-jumps to 0x004DFFC3 (its next body; owner is the address-named
//   stand-in ??4CBase_00431f10_Derived_004dffc3@@QAEAAU0@ABU0@@Z); Ego_r/FableWin name set; its body opens with a call of the ancestor
//   operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CBedDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CBedDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local class
//   over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CBedDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CBedDef& operator=(const CBedDef& other);
};

void CBedDef::Copy(const CDefClassBase* other)
{
    *this = *(const CBedDef*)other;
}
