// CCreatureAction_GetOutOfBed::Clone() const -- `return new CCreatureAction_GetOutOfBed(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_GetOutOfBed@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00860210, 34 bytes: operator new(0xB0) 0x00BFEA1A, null test, then the copy constructor 0x00860240 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_GetOutOfBed@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_GetOutOfBed@@QAE@ABV0@@Z: FableWin name set; retail 0x00860240 stores
//   CCreatureAction_GetOutOfBed's vftable 0x01276B0C first; the landed row at 0x00860240 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_GetOutOfBed@@QAEPAVCCreatureActionBase@@XZ (non-virtual, non-const);
//   now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_GetOutOfBed.h does not give the class at sizeof 0xB0 with Clone declared in the prelude
//   tried (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_GetOutOfBed : public CCreatureActionBase
{
public:
    CCreatureAction_GetOutOfBed(const CCreatureAction_GetOutOfBed& other);   // 0x00860240
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_GetOutOfBed@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB0 - sizeof(CCreatureActionBase)];   // CCreatureAction_GetOutOfBed's own members (not modelled: the copy constructor is out of line); sizeof 0xB0
};

CCreatureActionBase* CCreatureAction_GetOutOfBed::Clone() const
{
    return new CCreatureAction_GetOutOfBed(*this);
}
