// CCreatureAction_MagicBlast::Clone() const -- `return new CCreatureAction_MagicBlast(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_MagicBlast@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009826A0, 34 bytes: operator new(0xB0) 0x00BFEA1A, null test, then the copy constructor 0x009826D0 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_MagicBlast@@UBEPAVCCreatureActionBase@@XZ: RTTI placement + Ego_r/FableWin;
//   callee ??0CCreatureAction_MagicBlast@@QAE@ABV0@@Z: FableWin name set; retail 0x009826D0 stores CCreatureAction_MagicBlast's vftable
//   0x0129935C first; the landed row at 0x009826D0 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_MagicBlast@@QAEPAVCCreatureActionBase@@XZ (non-virtual, non-const); now
//   the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CCreatureAction_MagicBlast; a local class over its RTTI direct base CCreatureActionBase
//   engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_MagicBlast : public CCreatureActionBase
{
public:
    CCreatureAction_MagicBlast(const CCreatureAction_MagicBlast& other);   // 0x009826D0
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_MagicBlast@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB0 - sizeof(CCreatureActionBase)];   // CCreatureAction_MagicBlast's own members (not modelled: the copy constructor is out of line); sizeof 0xB0
};

CCreatureActionBase* CCreatureAction_MagicBlast::Clone() const
{
    return new CCreatureAction_MagicBlast(*this);
}
