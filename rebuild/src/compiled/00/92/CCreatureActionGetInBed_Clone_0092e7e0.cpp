// CCreatureAction_GetInBed::Clone() const -- `return new CCreatureAction_GetInBed(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_GetInBed@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092E7E0, 34 bytes: operator new(0xC0) 0x00BFEA1A, null test, then the copy constructor 0x0092E810 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_GetInBed@@UBEPAVCCreatureActionBase@@XZ: RTTI placement + Ego_r/FableWin;
//   callee ??0CCreatureAction_GetInBed@@QAE@ABV0@@Z: Ego_r/FableWin name set; retail 0x0092E810 stores CCreatureAction_GetInBed's
//   vftable 0x01291BFC first; the landed row at 0x0092E810 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_GetInBed@@QAEPAVCCreatureActionBase@@XZ (non-virtual, non-const); now
//   the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_GetInBed.h does not give the class at sizeof 0xC0 with Clone declared in the prelude
//   tried (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_GetInBed : public CCreatureActionBase
{
public:
    CCreatureAction_GetInBed(const CCreatureAction_GetInBed& other);   // 0x0092E810
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_GetInBed@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xC0 - sizeof(CCreatureActionBase)];   // CCreatureAction_GetInBed's own members (not modelled: the copy constructor is out of line); sizeof 0xC0
};

CCreatureActionBase* CCreatureAction_GetInBed::Clone() const
{
    return new CCreatureAction_GetInBed(*this);
}
