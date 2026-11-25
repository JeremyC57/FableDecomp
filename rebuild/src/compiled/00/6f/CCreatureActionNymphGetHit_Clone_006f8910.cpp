// CCreatureAction_NymphGetHit::Clone() const -- `return new CCreatureAction_NymphGetHit(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_NymphGetHit@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F8910, 34 bytes: operator new(0x154) 0x00BFEA1A, null test, then the copy constructor 0x006F8940 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_NymphGetHit@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_NymphGetHit@@QAE@ABV0@@Z: FableWin name set; retail 0x006F8940 stores
//   CCreatureAction_NymphGetHit's vftable 0x01263F84 first; the landed row at 0x006F8940 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_NymphGetHit@@QAEPAVCCreatureActionBase@@XZ (non-virtual, non-const);
//   now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_NymphGetHit.h does not give the class at sizeof 0x154 with Clone declared in the prelude
//   tried (header debt); a local class over its RTTI direct base CCombatAction_GenericStrikeResponseBase
//   engine/CCombatAction_GenericStrikeResponseBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCombatAction_GenericStrikeResponseBase.h"

class CCreatureAction_NymphGetHit : public CCombatAction_GenericStrikeResponseBase
{
public:
    CCreatureAction_NymphGetHit(const CCreatureAction_NymphGetHit& other);   // 0x006F8940
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_NymphGetHit@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0x154 - sizeof(CCombatAction_GenericStrikeResponseBase)];   // CCreatureAction_NymphGetHit's own members (not modelled: the copy constructor is out of line); sizeof 0x154
};

CCreatureActionBase* CCreatureAction_NymphGetHit::Clone() const
{
    return new CCreatureAction_NymphGetHit(*this);
}
