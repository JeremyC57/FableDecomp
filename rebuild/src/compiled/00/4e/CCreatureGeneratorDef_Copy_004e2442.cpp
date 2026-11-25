// CCreatureGeneratorDef::Copy(const CDefClassBase* other) -- copy every member of another CCreatureGeneratorDef: `*this = *(const CCreatureGeneratorDef*)other;`
// Leaf: ?Copy@CCreatureGeneratorDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E2442, 5 bytes: a thiscall tail call, `jmp` to CCreatureGeneratorDef::operator= 0x004E2447.
// Identity (NF425, 2026-11-25): self ?Copy@CCreatureGeneratorDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CCreatureGeneratorDef@@QAEAAV0@ABV0@@Z: bound at 0x004E2447 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of
//   the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CCreatureGeneratorDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CCreatureGeneratorDef; a local class over its RTTI direct base CSubDefClassBase
//   engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CCreatureGeneratorDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CCreatureGeneratorDef& operator=(const CCreatureGeneratorDef& other);
};

void CCreatureGeneratorDef::Copy(const CDefClassBase* other)
{
    *this = *(const CCreatureGeneratorDef*)other;
}
