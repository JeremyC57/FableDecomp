// CCreatureAction_BlowHorn::Clone() const -- `return new CCreatureAction_BlowHorn(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_BlowHorn@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084C290, 34 bytes: operator new(0xB0) 0x00BFEA1A, null test, then the copy constructor 0x0084C2C0 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_BlowHorn@@UBEPAVCCreatureActionBase@@XZ: RTTI placement + Ego_r/FableWin;
//   callee ??0CCreatureAction_BlowHorn@@QAE@ABV0@@Z: FableWin name set; retail 0x0084C2C0 stores CCreatureAction_BlowHorn's vftable
//   0x0127428C first; the landed row at 0x0084C2C0 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_BlowHorn@@QAEPAVCCreatureActionBase@@XZ (non-virtual, non-const); now
//   the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_BlowHorn.h does not give the class at sizeof 0xB0 with Clone declared in the prelude
//   tried (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_BlowHorn : public CCreatureActionBase
{
public:
    CCreatureAction_BlowHorn(const CCreatureAction_BlowHorn& other);   // 0x0084C2C0
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_BlowHorn@@UAEXXZ (FableWin name set; the base's is pure and retail instantiates the class)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB0 - sizeof(CCreatureActionBase)];   // CCreatureAction_BlowHorn's own members (not modelled: the copy constructor is out of line); sizeof 0xB0
};

CCreatureActionBase* CCreatureAction_BlowHorn::Clone() const
{
    return new CCreatureAction_BlowHorn(*this);
}
