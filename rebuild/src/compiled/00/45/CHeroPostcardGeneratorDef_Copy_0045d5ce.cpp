// CHeroPostcardGeneratorDef::Copy(const CDefClassBase* other) -- copy every member of another CHeroPostcardGeneratorDef: `*this = *(const CHeroPostcardGeneratorDef*)other;`
// Leaf: ?Copy@CHeroPostcardGeneratorDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045D5CE, 5 bytes: a thiscall tail call, `jmp` to CHeroPostcardGeneratorDef::operator= 0x0045D5D3.
// Identity (NF425, 2026-11-25): self ?Copy@CHeroPostcardGeneratorDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CHeroPostcardGeneratorDef@@QAEAAV0@ABV0@@Z: bound at 0x0045D5D3 (owner Leaf); Ego_r/FableWin name set; its body opens with a
//   call of the ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CHeroPostcardGeneratorDef_Copy; now the real virtual member, so the RTTI-placed name is
//   defined.
// Header: no engine header defines CHeroPostcardGeneratorDef; a local class over its RTTI direct base CParentDefClassBase
//   engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CHeroPostcardGeneratorDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CHeroPostcardGeneratorDef& operator=(const CHeroPostcardGeneratorDef& other);
};

void CHeroPostcardGeneratorDef::Copy(const CDefClassBase* other)
{
    *this = *(const CHeroPostcardGeneratorDef*)other;
}
