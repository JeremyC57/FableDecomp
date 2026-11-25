// CCreatureAction_PickCrop::Clone() const -- `return new CCreatureAction_PickCrop(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_PickCrop@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009406B0, 34 bytes: operator new(0xB0) 0x00BFEA1A, null test, then the copy constructor 0x009406E0 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_PickCrop@@UBEPAVCCreatureActionBase@@XZ: RTTI placement + Ego_r/FableWin;
//   callee ??0CCreatureAction_PickCrop@@QAE@ABV0@@Z: FableWin name set; retail 0x009406E0 stores CCreatureAction_PickCrop's vftable
//   0x012925F4 first; the landed row at 0x009406E0 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_PickCrop@@QAEPAVCCreatureActionBase@@XZ (non-virtual, non-const); now
//   the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_PickCrop.h does not give the class at sizeof 0xB0 with Clone declared in the prelude
//   tried (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_PickCrop : public CCreatureActionBase
{
public:
    CCreatureAction_PickCrop(const CCreatureAction_PickCrop& other);   // 0x009406E0
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_PickCrop@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB0 - sizeof(CCreatureActionBase)];   // CCreatureAction_PickCrop's own members (not modelled: the copy constructor is out of line); sizeof 0xB0
};

CCreatureActionBase* CCreatureAction_PickCrop::Clone() const
{
    return new CCreatureAction_PickCrop(*this);
}
