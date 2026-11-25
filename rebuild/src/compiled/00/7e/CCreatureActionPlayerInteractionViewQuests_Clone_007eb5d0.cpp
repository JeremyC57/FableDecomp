// CCreatureAction_PlayerInteractionViewQuests::Clone() const -- `return new CCreatureAction_PlayerInteractionViewQuests(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_PlayerInteractionViewQuests@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EB5D0, 34 bytes: operator new(0xB0) 0x00BFEA1A, null test, then the copy constructor 0x007EB600 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_PlayerInteractionViewQuests@@UBEPAVCCreatureActionBase@@XZ: RTTI placement
//   + Ego_r/FableWin; callee ??0CCreatureAction_PlayerInteractionViewQuests@@QAE@ABV0@@Z: FableWin name set; retail 0x007EB600 stores
//   CCreatureAction_PlayerInteractionViewQuests's vftable 0x01270E7C first; the landed row at 0x007EB600 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_PlayerInteractionViewQuests@@QAEPAVCCreatureActionBase@@XZ
//   (non-virtual, non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_PlayerInteractionViewQuests.h does not give the class at sizeof 0xB0 with Clone declared
//   in the prelude tried (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_PlayerInteractionViewQuests : public CCreatureActionBase
{
public:
    CCreatureAction_PlayerInteractionViewQuests(const CCreatureAction_PlayerInteractionViewQuests& other);   // 0x007EB600
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_PlayerInteractionViewQuests@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB0 - sizeof(CCreatureActionBase)];   // CCreatureAction_PlayerInteractionViewQuests's own members (not modelled: the copy constructor is out of line); sizeof 0xB0
};

CCreatureActionBase* CCreatureAction_PlayerInteractionViewQuests::Clone() const
{
    return new CCreatureAction_PlayerInteractionViewQuests(*this);
}
