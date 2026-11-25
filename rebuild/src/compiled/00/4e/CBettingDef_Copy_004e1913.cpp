// CBettingDef::Copy(const CDefClassBase* other) -- copy every member of another CBettingDef: `*this = *(const CBettingDef*)other;`
// Leaf: ?Copy@CBettingDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E1913, 5 bytes: a thiscall tail call, `jmp` to CBettingDef::operator= 0x004E1918.
// Identity (NF425, 2026-11-25): self ?Copy@CBettingDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CBettingDef@@QAEAAV0@ABV0@@Z: bound at 0x004E1918 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CBettingDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CBettingDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local class
//   over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CBettingDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CBettingDef& operator=(const CBettingDef& other);
};

void CBettingDef::Copy(const CDefClassBase* other)
{
    *this = *(const CBettingDef*)other;
}
