// CHeroSpecialMovementDef::Copy(const CDefClassBase* other) -- copy every member of another CHeroSpecialMovementDef: `*this = *(const CHeroSpecialMovementDef*)other;`
// Leaf: ?Copy@CHeroSpecialMovementDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E1362, 5 bytes: a thiscall tail call, `jmp` to CHeroSpecialMovementDef::operator= 0x004E1367.
// Identity (NF425, 2026-11-25): self ?Copy@CHeroSpecialMovementDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CHeroSpecialMovementDef@@QAEAAV0@ABV0@@Z: the RTTI-proven Copy of CHeroSpecialMovementDef tail-jumps to 0x004E1367 (its next
//   body; owner is the address-named stand-in ??4CBase_00431f10_Derived_004e1367@@QAEAAU0@ABU0@@Z); Ego_r/FableWin name set; its body
//   opens with a call of the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CHeroSpecialMovementDef_Copy; now the real virtual member, so the RTTI-placed name is
//   defined.
// Header: engine header engine/CHeroSpecialMovementDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a
//   local class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CHeroSpecialMovementDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CHeroSpecialMovementDef& operator=(const CHeroSpecialMovementDef& other);
};

void CHeroSpecialMovementDef::Copy(const CDefClassBase* other)
{
    *this = *(const CHeroSpecialMovementDef*)other;
}
