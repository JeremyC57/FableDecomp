// CCreatureAction_SpiritAttack::Clone() const -- `return new CCreatureAction_SpiritAttack(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_SpiritAttack@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00855230, 34 bytes: operator new(0xB8) 0x00BFEA1A, null test, then the copy constructor 0x00855260 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_SpiritAttack@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_SpiritAttack@@QAE@ABV0@@Z: Ego_r/FableWin name set; retail 0x00855260 stores
//   CCreatureAction_SpiritAttack's vftable 0x01275D74 first; the landed row at 0x00855260 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_SpiritAttack@@QAEPAVCCreatureActionBase@@XZ (non-virtual, non-const);
//   now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_SpiritAttack.h does not give the class at sizeof 0xB8 with Clone declared in the prelude
//   tried (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_SpiritAttack : public CCreatureActionBase
{
public:
    CCreatureAction_SpiritAttack(const CCreatureAction_SpiritAttack& other);   // 0x00855260
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_SpiritAttack@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB8 - sizeof(CCreatureActionBase)];   // CCreatureAction_SpiritAttack's own members (not modelled: the copy constructor is out of line); sizeof 0xB8
};

CCreatureActionBase* CCreatureAction_SpiritAttack::Clone() const
{
    return new CCreatureAction_SpiritAttack(*this);
}
