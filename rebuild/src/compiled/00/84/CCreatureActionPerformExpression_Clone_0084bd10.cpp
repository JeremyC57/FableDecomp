// CCreatureAction_PerformExpression::Clone() const -- `return new CCreatureAction_PerformExpression(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_PerformExpression@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084BD10, 34 bytes: operator new(0xC8) 0x00BFEA1A, null test, then the copy constructor 0x0084BD40 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_PerformExpression@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_PerformExpression@@QAE@ABV0@@Z: Ego_r/FableWin name set; retail 0x0084BD40 stores
//   CCreatureAction_PerformExpression's vftable 0x012743DC first; the landed row at 0x0084BD40 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_PerformExpression@@QAEPAVCCreatureActionBase@@XZ (non-virtual,
//   non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_PerformExpression.h does not give the class at sizeof 0xC8 with Clone declared in the
//   prelude tried (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_PerformExpression : public CCreatureActionBase
{
public:
    CCreatureAction_PerformExpression(const CCreatureAction_PerformExpression& other);   // 0x0084BD40
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_PerformExpression@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xC8 - sizeof(CCreatureActionBase)];   // CCreatureAction_PerformExpression's own members (not modelled: the copy constructor is out of line); sizeof 0xC8
};

CCreatureActionBase* CCreatureAction_PerformExpression::Clone() const
{
    return new CCreatureAction_PerformExpression(*this);
}
