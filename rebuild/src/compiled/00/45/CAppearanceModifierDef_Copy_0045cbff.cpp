// CAppearanceModifierDef::Copy(const CDefClassBase* other) -- copy every member of another CAppearanceModifierDef: `*this = *(const CAppearanceModifierDef*)other;`
// Leaf: ?Copy@CAppearanceModifierDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045CBFF, 5 bytes: a thiscall tail call, `jmp` to CAppearanceModifierDef::operator= 0x0045CC04.
// Identity (NF425, 2026-11-25): self ?Copy@CAppearanceModifierDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CAppearanceModifierDef@@QAEAAV0@ABV0@@Z: bound at 0x0045CC04 (owner Leaf); Ego_r/FableWin name set; its body opens with a call
//   of the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CAppearanceModifierDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CAppearanceModifierDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a
//   local class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CAppearanceModifierDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CAppearanceModifierDef& operator=(const CAppearanceModifierDef& other);
};

void CAppearanceModifierDef::Copy(const CDefClassBase* other)
{
    *this = *(const CAppearanceModifierDef*)other;
}
