// CBossDef::Copy(const CDefClassBase* other) -- copy every member of another CBossDef: `*this = *(const CBossDef*)other;`
// Leaf: ?Copy@CBossDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E2C03, 5 bytes: a thiscall tail call, `jmp` to CBossDef::operator= 0x004E2C08.
// Identity (NF425, 2026-11-25): self ?Copy@CBossDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CBossDef@@QAEAAV0@ABV0@@Z: bound at 0x004E2C08 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the ancestor
//   operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CBossDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CBossDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local class
//   over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CBossDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CBossDef& operator=(const CBossDef& other);
};

void CBossDef::Copy(const CDefClassBase* other)
{
    *this = *(const CBossDef*)other;
}
