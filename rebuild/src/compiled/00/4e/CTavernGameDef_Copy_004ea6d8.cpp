// CTavernGameDef::Copy(const CDefClassBase* other) -- copy every member of another CTavernGameDef: `*this = *(const CTavernGameDef*)other;`
// Leaf: ?Copy@CTavernGameDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EA6D8, 5 bytes: a thiscall tail call, `jmp` to CTavernGameDef::operator= 0x004EA6DD.
// Identity (NF425, 2026-11-25): self ?Copy@CTavernGameDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CTavernGameDef@@QAEAAV0@ABV0@@Z: bound at 0x004EA6DD (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CTavernGameDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CTavernGameDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CTavernGameDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CTavernGameDef& operator=(const CTavernGameDef& other);
};

void CTavernGameDef::Copy(const CDefClassBase* other)
{
    *this = *(const CTavernGameDef*)other;
}
