// CCreatureAction_LearnExpression::Clone() const -- `return new CCreatureAction_LearnExpression(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_LearnExpression@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084BED0, 34 bytes: operator new(0xB8) 0x00BFEA1A, null test, then the copy constructor 0x0084BF00 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_LearnExpression@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_LearnExpression@@QAE@ABV0@@Z: Ego_r/FableWin name set; retail 0x0084BF00 stores
//   CCreatureAction_LearnExpression's vftable 0x0127461C first; the landed row at 0x0084BF00 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_LearnExpression@@QAEPAVCCreatureActionBase@@XZ (non-virtual,
//   non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CCreatureAction_LearnExpression; a local class over its RTTI direct base CCreatureActionBase
//   engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_LearnExpression : public CCreatureActionBase
{
public:
    CCreatureAction_LearnExpression(const CCreatureAction_LearnExpression& other);   // 0x0084BF00
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_LearnExpression@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB8 - sizeof(CCreatureActionBase)];   // CCreatureAction_LearnExpression's own members (not modelled: the copy constructor is out of line); sizeof 0xB8
};

CCreatureActionBase* CCreatureAction_LearnExpression::Clone() const
{
    return new CCreatureAction_LearnExpression(*this);
}
