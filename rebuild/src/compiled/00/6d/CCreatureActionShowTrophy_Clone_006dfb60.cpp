// CCreatureAction_ShowTrophy::Clone() const -- `return new CCreatureAction_ShowTrophy(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_ShowTrophy@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DFB60, 34 bytes: operator new(0xC0) 0x00BFEA1A, null test, then the copy constructor 0x006DFB90 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_ShowTrophy@@UBEPAVCCreatureActionBase@@XZ: RTTI placement + Ego_r/FableWin;
//   callee ??0CCreatureAction_ShowTrophy@@QAE@ABV0@@Z: Ego_r/FableWin name set; retail 0x006DFB90 stores CCreatureAction_ShowTrophy's
//   vftable 0x0125F7B4 first; the landed row at 0x006DFB90 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_ShowTrophy@@QAEPAVCCreatureActionBase@@XZ (non-virtual, non-const); now
//   the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_ShowTrophy.h does not give the class at sizeof 0xC0 with Clone declared in the prelude
//   tried (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_ShowTrophy : public CCreatureActionBase
{
public:
    CCreatureAction_ShowTrophy(const CCreatureAction_ShowTrophy& other);   // 0x006DFB90
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_ShowTrophy@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xC0 - sizeof(CCreatureActionBase)];   // CCreatureAction_ShowTrophy's own members (not modelled: the copy constructor is out of line); sizeof 0xC0
};

CCreatureActionBase* CCreatureAction_ShowTrophy::Clone() const
{
    return new CCreatureAction_ShowTrophy(*this);
}
