// CCreatureAction_ShowItemToThing::Clone() const -- `return new CCreatureAction_ShowItemToThing(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_ShowItemToThing@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007ED9F0, 34 bytes: operator new(0xB8) 0x00BFEA1A, null test, then the copy constructor 0x007EDA20 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_ShowItemToThing@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_ShowItemToThing@@QAE@ABV0@@Z: Ego_r/FableWin name set; retail 0x007EDA20 stores
//   CCreatureAction_ShowItemToThing's vftable 0x012714C4 first; the landed row at 0x007EDA20 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_ShowItemToThing@@QAEPAVCCreatureActionBase@@XZ (non-virtual,
//   non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CCreatureAction_ShowItemToThing; a local class over its RTTI direct base CCreatureActionBase
//   engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_ShowItemToThing : public CCreatureActionBase
{
public:
    CCreatureAction_ShowItemToThing(const CCreatureAction_ShowItemToThing& other);   // 0x007EDA20
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_ShowItemToThing@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB8 - sizeof(CCreatureActionBase)];   // CCreatureAction_ShowItemToThing's own members (not modelled: the copy constructor is out of line); sizeof 0xB8
};

CCreatureActionBase* CCreatureAction_ShowItemToThing::Clone() const
{
    return new CCreatureAction_ShowItemToThing(*this);
}
