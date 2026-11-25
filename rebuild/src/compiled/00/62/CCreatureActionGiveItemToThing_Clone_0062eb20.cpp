// CCreatureAction_GiveItemToThing::Clone() const -- `return new CCreatureAction_GiveItemToThing(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_GiveItemToThing@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062EB20, 34 bytes: operator new(0xB8) 0x00BFEA1A, null test, then the copy constructor 0x0062EB50 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_GiveItemToThing@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_GiveItemToThing@@QAE@ABV0@@Z: Ego_r/FableWin name set; retail 0x0062EB50 stores
//   CCreatureAction_GiveItemToThing's vftable 0x0125919C first; the landed row at 0x0062EB50 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_GiveItemToThing@@QAEPAVCCreatureActionBase@@XZ (non-virtual,
//   non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_GiveItemToThing.h does not give the class at sizeof 0xB8 with Clone declared in the
//   prelude tried (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_GiveItemToThing : public CCreatureActionBase
{
public:
    CCreatureAction_GiveItemToThing(const CCreatureAction_GiveItemToThing& other);   // 0x0062EB50
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_GiveItemToThing@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB8 - sizeof(CCreatureActionBase)];   // CCreatureAction_GiveItemToThing's own members (not modelled: the copy constructor is out of line); sizeof 0xB8
};

CCreatureActionBase* CCreatureAction_GiveItemToThing::Clone() const
{
    return new CCreatureAction_GiveItemToThing(*this);
}
