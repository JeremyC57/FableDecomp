// CCreatureAction_SpiritHitResponse::Clone() const -- `return new CCreatureAction_SpiritHitResponse(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_SpiritHitResponse@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00855190, 34 bytes: operator new(0xB4) 0x00BFEA1A, null test, then the copy constructor 0x008551C0 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_SpiritHitResponse@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_SpiritHitResponse@@QAE@ABV0@@Z: FableWin name set; retail 0x008551C0 stores
//   CCreatureAction_SpiritHitResponse's vftable 0x01275B34 first; the landed row at 0x008551C0 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_SpiritHitResponse@@QAEPAVCCreatureActionBase@@XZ (non-virtual,
//   non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_SpiritHitResponse.h does not give the class at sizeof 0xB4 with Clone declared in the
//   prelude tried (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_SpiritHitResponse : public CCreatureActionBase
{
public:
    CCreatureAction_SpiritHitResponse(const CCreatureAction_SpiritHitResponse& other);   // 0x008551C0
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_SpiritHitResponse@@UAEXXZ (FableWin name set; the base's is pure and retail instantiates the class)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB4 - sizeof(CCreatureActionBase)];   // CCreatureAction_SpiritHitResponse's own members (not modelled: the copy constructor is out of line); sizeof 0xB4
};

CCreatureActionBase* CCreatureAction_SpiritHitResponse::Clone() const
{
    return new CCreatureAction_SpiritHitResponse(*this);
}
