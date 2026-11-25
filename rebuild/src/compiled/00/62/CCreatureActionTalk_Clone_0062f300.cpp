// CCreatureAction_Talk::Clone() const -- `return new CCreatureAction_Talk(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_Talk@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062F300, 34 bytes: operator new(0xB0) 0x00BFEA1A, null test, then the copy constructor 0x0062F330 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_Talk@@UBEPAVCCreatureActionBase@@XZ: RTTI placement + Ego_r/FableWin;
//   callee ??0CCreatureAction_Talk@@QAE@ABV0@@Z: FableWin name set; retail 0x0062F330 stores CCreatureAction_Talk's vftable 0x012599E4
//   first; the landed row at 0x0062F330 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_Talk@@QAEPAVCCreatureActionBase@@XZ (non-virtual, non-const); now the
//   real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CCreatureAction_Talk; a local class over its RTTI direct base CCreatureActionBase
//   engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_Talk : public CCreatureActionBase
{
public:
    CCreatureAction_Talk(const CCreatureAction_Talk& other);   // 0x0062F330
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_Talk@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB0 - sizeof(CCreatureActionBase)];   // CCreatureAction_Talk's own members (not modelled: the copy constructor is out of line); sizeof 0xB0
};

CCreatureActionBase* CCreatureAction_Talk::Clone() const
{
    return new CCreatureAction_Talk(*this);
}
