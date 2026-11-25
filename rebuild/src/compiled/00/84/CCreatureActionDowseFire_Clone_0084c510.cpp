// CCreatureAction_DowseFire::Clone() const -- `return new CCreatureAction_DowseFire(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_DowseFire@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084C510, 34 bytes: operator new(0xBC) 0x00BFEA1A, null test, then the copy constructor 0x0084C540 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_DowseFire@@UBEPAVCCreatureActionBase@@XZ: RTTI placement + Ego_r/FableWin;
//   callee ??0CCreatureAction_DowseFire@@QAE@ABV0@@Z: Ego_r/FableWin name set; retail 0x0084C540 stores CCreatureAction_DowseFire's
//   vftable 0x01274D0C first; the landed row at 0x0084C540 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_DowseFire@@QAEPAVCCreatureActionBase@@XZ (non-virtual, non-const); now
//   the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_DowseFire.h does not give the class at sizeof 0xBC with Clone declared in the prelude
//   tried (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_DowseFire : public CCreatureActionBase
{
public:
    CCreatureAction_DowseFire(const CCreatureAction_DowseFire& other);   // 0x0084C540
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_DowseFire@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xBC - sizeof(CCreatureActionBase)];   // CCreatureAction_DowseFire's own members (not modelled: the copy constructor is out of line); sizeof 0xBC
};

CCreatureActionBase* CCreatureAction_DowseFire::Clone() const
{
    return new CCreatureAction_DowseFire(*this);
}
