// CMaterialDef::Copy(const CDefClassBase* other) -- copy every member of another CMaterialDef: `*this = *(const CMaterialDef*)other;`
// Leaf: ?Copy@CMaterialDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045C10D, 5 bytes: a thiscall tail call, `jmp` to CMaterialDef::operator= 0x0045C112.
// Identity (NF425, 2026-11-25): self ?Copy@CMaterialDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CMaterialDef@@QAEAAV0@ABV0@@Z: bound at 0x0045C112 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CMaterialDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CMaterialDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CParentDefClassBase engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CMaterialDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CMaterialDef& operator=(const CMaterialDef& other);
};

void CMaterialDef::Copy(const CDefClassBase* other)
{
    *this = *(const CMaterialDef*)other;
}
