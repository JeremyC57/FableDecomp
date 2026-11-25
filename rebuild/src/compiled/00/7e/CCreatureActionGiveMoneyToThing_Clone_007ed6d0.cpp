// CCreatureAction_GiveMoneyToThing::Clone() const -- `return new CCreatureAction_GiveMoneyToThing(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_GiveMoneyToThing@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007ED6D0, 34 bytes: operator new(0xB4) 0x00BFEA1A, null test, then the copy constructor 0x007ED700 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_GiveMoneyToThing@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_GiveMoneyToThing@@QAE@ABV0@@Z: FableWin name set; retail 0x007ED700 stores
//   CCreatureAction_GiveMoneyToThing's vftable 0x0127139C first; the landed row at 0x007ED700 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_GiveMoneyToThing@@QAEPAVCCreatureActionBase@@XZ (non-virtual,
//   non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CCreatureAction_GiveMoneyToThing; a local class over its RTTI direct base CCreatureActionBase
//   engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_GiveMoneyToThing : public CCreatureActionBase
{
public:
    CCreatureAction_GiveMoneyToThing(const CCreatureAction_GiveMoneyToThing& other);   // 0x007ED700
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_GiveMoneyToThing@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB4 - sizeof(CCreatureActionBase)];   // CCreatureAction_GiveMoneyToThing's own members (not modelled: the copy constructor is out of line); sizeof 0xB4
};

CCreatureActionBase* CCreatureAction_GiveMoneyToThing::Clone() const
{
    return new CCreatureAction_GiveMoneyToThing(*this);
}
