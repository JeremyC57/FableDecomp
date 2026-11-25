// CCreatureAction_PerformExpressionExtended::Clone() const -- `return new CCreatureAction_PerformExpressionExtended(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_PerformExpressionExtended@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084BE00, 34 bytes: operator new(0xC8) 0x00BFEA1A, null test, then the copy constructor 0x0084BE30 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_PerformExpressionExtended@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_PerformExpressionExtended@@QAE@ABV0@@Z: Ego_r/FableWin name set; retail 0x0084BE30 stores
//   CCreatureAction_PerformExpressionExtended's vftable 0x012744FC first; the landed row at 0x0084BE30 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_PerformExpressionExtended@@QAEPAVCCreatureActionBase@@XZ (non-virtual,
//   non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_PerformExpressionExtended.h does not give the class at sizeof 0xC8 with Clone declared
//   in the prelude tried (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_PerformExpressionExtended : public CCreatureActionBase
{
public:
    CCreatureAction_PerformExpressionExtended(const CCreatureAction_PerformExpressionExtended& other);   // 0x0084BE30
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_PerformExpressionExtended@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xC8 - sizeof(CCreatureActionBase)];   // CCreatureAction_PerformExpressionExtended's own members (not modelled: the copy constructor is out of line); sizeof 0xC8
};

CCreatureActionBase* CCreatureAction_PerformExpressionExtended::Clone() const
{
    return new CCreatureAction_PerformExpressionExtended(*this);
}
