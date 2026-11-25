// CCreatureGenerationFamilyDef::Copy(const CDefClassBase* other) -- copy every member of another CCreatureGenerationFamilyDef: `*this = *(const CCreatureGenerationFamilyDef*)other;`
// Leaf: ?Copy@CCreatureGenerationFamilyDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045C60F, 5 bytes: a thiscall tail call, `jmp` to CCreatureGenerationFamilyDef::operator= 0x0045C614.
// Identity (NF425, 2026-11-25): self ?Copy@CCreatureGenerationFamilyDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin;
//   target ??4CCreatureGenerationFamilyDef@@QAEAAV0@ABV0@@Z: bound at 0x0045C614 (owner Leaf); Ego_r/FableWin name set; its body opens
//   with a call of the ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CCreatureGenerationFamilyDef_Copy; now the real virtual member, so the RTTI-placed name is
//   defined.
// Header: engine header engine/CCreatureGenerationFamilyDef.h does not declare Copy / operator= in any landed prelude tried (header
//   debt); a local class over its RTTI direct base CParentDefClassBase engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CCreatureGenerationFamilyDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CCreatureGenerationFamilyDef& operator=(const CCreatureGenerationFamilyDef& other);
};

void CCreatureGenerationFamilyDef::Copy(const CDefClassBase* other)
{
    *this = *(const CCreatureGenerationFamilyDef*)other;
}
