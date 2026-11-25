// CTavernGameSpotTheAdditionDef::Copy(const CDefClassBase* other) -- copy every member of another CTavernGameSpotTheAdditionDef: `*this = *(const CTavernGameSpotTheAdditionDef*)other;`
// Leaf: ?Copy@CTavernGameSpotTheAdditionDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E2E08, 5 bytes: a thiscall tail call, `jmp` to CTavernGameSpotTheAdditionDef::operator= 0x004E2E0D.
// Identity (NF425, 2026-11-25): self ?Copy@CTavernGameSpotTheAdditionDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin;
//   target ??4CTavernGameSpotTheAdditionDef@@QAEAAV0@ABV0@@Z: bound at 0x004E2E0D (owner Leaf); Ego_r/FableWin name set; its body opens
//   with a call of the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CTavernGameSpotTheAdditionDef_Copy; now the real virtual member, so the RTTI-placed name is
//   defined.
// Header: engine header engine/CTavernGameSpotTheAdditionDef.h does not declare Copy / operator= in any landed prelude tried (header
//   debt); a local class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CTavernGameSpotTheAdditionDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CTavernGameSpotTheAdditionDef& operator=(const CTavernGameSpotTheAdditionDef& other);
};

void CTavernGameSpotTheAdditionDef::Copy(const CDefClassBase* other)
{
    *this = *(const CTavernGameSpotTheAdditionDef*)other;
}
