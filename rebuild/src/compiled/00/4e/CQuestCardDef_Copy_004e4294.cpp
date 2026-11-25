// CQuestCardDef::Copy(const CDefClassBase* other) -- copy every member of another CQuestCardDef: `*this = *(const CQuestCardDef*)other;`
// Leaf: ?Copy@CQuestCardDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E4294, 5 bytes: a thiscall tail call, `jmp` to CQuestCardDef::operator= 0x004E4299.
// Identity (NF425, 2026-11-25): self ?Copy@CQuestCardDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CQuestCardDef@@QAEAAV0@ABV0@@Z: bound at 0x004E4299 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CQuestCardDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CQuestCardDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CQuestCardDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CQuestCardDef& operator=(const CQuestCardDef& other);
};

void CQuestCardDef::Copy(const CDefClassBase* other)
{
    *this = *(const CQuestCardDef*)other;
}
