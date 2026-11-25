// CCreatureAction_PickUpJugToFill::Clone() const -- `return new CCreatureAction_PickUpJugToFill(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_PickUpJugToFill@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084CA90, 34 bytes: operator new(0xB0) 0x00BFEA1A, null test, then the copy constructor 0x0084CAC0 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_PickUpJugToFill@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_PickUpJugToFill@@QAE@ABV0@@Z: FableWin name set; retail 0x0084CAC0 stores
//   CCreatureAction_PickUpJugToFill's vftable 0x01275504 first; the landed row at 0x0084CAC0 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_PickUpJugToFill@@QAEPAVCCreatureActionBase@@XZ (non-virtual,
//   non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CCreatureAction_PickUpJugToFill; a local class over its RTTI direct base CCreatureActionBase
//   engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_PickUpJugToFill : public CCreatureActionBase
{
public:
    CCreatureAction_PickUpJugToFill(const CCreatureAction_PickUpJugToFill& other);   // 0x0084CAC0
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_PickUpJugToFill@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB0 - sizeof(CCreatureActionBase)];   // CCreatureAction_PickUpJugToFill's own members (not modelled: the copy constructor is out of line); sizeof 0xB0
};

CCreatureActionBase* CCreatureAction_PickUpJugToFill::Clone() const
{
    return new CCreatureAction_PickUpJugToFill(*this);
}
