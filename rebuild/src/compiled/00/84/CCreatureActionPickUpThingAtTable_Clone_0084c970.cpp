// CCreatureAction_PickUpThingAtTable::Clone() const -- `return new CCreatureAction_PickUpThingAtTable(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_PickUpThingAtTable@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084C970, 34 bytes: operator new(0xB4) 0x00BFEA1A, null test, then the copy constructor 0x0084C9A0 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_PickUpThingAtTable@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_PickUpThingAtTable@@QAE@ABV0@@Z: FableWin name set; retail 0x0084C9A0 stores
//   CCreatureAction_PickUpThingAtTable's vftable 0x012753E4 first; the landed row at 0x0084C9A0 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_PickUpThingAtTable@@QAEPAVCCreatureActionBase@@XZ (non-virtual,
//   non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_Tankard.h does not give the class at sizeof 0xB4 with Clone declared in the prelude
//   tried (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_PickUpThingAtTable : public CCreatureActionBase
{
public:
    CCreatureAction_PickUpThingAtTable(const CCreatureAction_PickUpThingAtTable& other);   // 0x0084C9A0
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_PickUpThingAtTable@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB4 - sizeof(CCreatureActionBase)];   // CCreatureAction_PickUpThingAtTable's own members (not modelled: the copy constructor is out of line); sizeof 0xB4
};

CCreatureActionBase* CCreatureAction_PickUpThingAtTable::Clone() const
{
    return new CCreatureAction_PickUpThingAtTable(*this);
}
