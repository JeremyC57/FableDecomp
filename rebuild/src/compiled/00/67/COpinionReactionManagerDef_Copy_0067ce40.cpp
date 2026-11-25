// COpinionReactionManagerDef::Copy(const CDefClassBase* other) -- copy every member of another COpinionReactionManagerDef: `*this = *(const COpinionReactionManagerDef*)other;`
// Leaf: ?Copy@COpinionReactionManagerDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067CE40, 5 bytes: a thiscall tail call, `jmp` to COpinionReactionManagerDef::operator= 0x0067CE50.
// Identity (NF425, 2026-11-25): self ?Copy@COpinionReactionManagerDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin;
//   target ??4COpinionReactionManagerDef@@QAEAAV0@ABV0@@Z: bound at 0x0067CE50 (owner Leaf); Ego_r/FableWin name set; its body opens
//   with a call of the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self ?Copy@CControlsDef@@QAEXPBV1@@Z; now the real virtual member, so the RTTI-placed name is
//   defined.
// Header: engine header engine/COpinionReactionManagerDef.h does not declare Copy / operator= in any landed prelude tried (header
//   debt); a local class over its RTTI direct base CParentDefClassBase engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class COpinionReactionManagerDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    COpinionReactionManagerDef& operator=(const COpinionReactionManagerDef& other);
};

void COpinionReactionManagerDef::Copy(const CDefClassBase* other)
{
    *this = *(const COpinionReactionManagerDef*)other;
}
