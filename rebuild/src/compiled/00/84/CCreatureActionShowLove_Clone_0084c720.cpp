// CCreatureAction_ShowLove::Clone() const -- `return new CCreatureAction_ShowLove(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_ShowLove@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084C720, 34 bytes: operator new(0xB0) 0x00BFEA1A, null test, then the copy constructor 0x0084C750 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_ShowLove@@UBEPAVCCreatureActionBase@@XZ: RTTI placement + Ego_r/FableWin;
//   callee ??0CCreatureAction_ShowLove@@QAE@ABV0@@Z: FableWin name set; retail 0x0084C750 stores CCreatureAction_ShowLove's vftable
//   0x01274F4C first; the landed row at 0x0084C750 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_ShowLove@@QAEPAVCCreatureActionBase@@XZ (non-virtual, non-const); now
//   the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_ShowLove.h does not give the class at sizeof 0xB0 with Clone declared in the prelude
//   tried (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_ShowLove : public CCreatureActionBase
{
public:
    CCreatureAction_ShowLove(const CCreatureAction_ShowLove& other);   // 0x0084C750
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_ShowLove@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB0 - sizeof(CCreatureActionBase)];   // CCreatureAction_ShowLove's own members (not modelled: the copy constructor is out of line); sizeof 0xB0
};

CCreatureActionBase* CCreatureAction_ShowLove::Clone() const
{
    return new CCreatureAction_ShowLove(*this);
}
