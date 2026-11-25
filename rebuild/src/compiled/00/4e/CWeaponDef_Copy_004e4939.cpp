// CWeaponDef::Copy(const CDefClassBase* other) -- copy every member of another CWeaponDef: `*this = *(const CWeaponDef*)other;`
// Leaf: ?Copy@CWeaponDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E4939, 5 bytes: a thiscall tail call, `jmp` to CWeaponDef::operator= 0x004E493E.
// Identity (NF425, 2026-11-25): self ?Copy@CWeaponDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CWeaponDef@@QAEAAV0@ABV0@@Z: bound at 0x004E493E (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CWeaponDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CWeaponDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local class
//   over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CWeaponDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CWeaponDef& operator=(const CWeaponDef& other);
};

void CWeaponDef::Copy(const CDefClassBase* other)
{
    *this = *(const CWeaponDef*)other;
}
