// CCreatureAction_UseSwitch::Clone() const -- `return new CCreatureAction_UseSwitch(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_UseSwitch@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EE130, 34 bytes: operator new(0xB0) 0x00BFEA1A, null test, then the copy constructor 0x007EE160 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_UseSwitch@@UBEPAVCCreatureActionBase@@XZ: RTTI placement + Ego_r/FableWin;
//   callee ??0CCreatureAction_UseSwitch@@QAE@ABV0@@Z: FableWin name set; retail 0x007EE160 stores CCreatureAction_UseSwitch's vftable
//   0x01271604 first; the landed row at 0x007EE160 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_UseSwitch@@QAEPAVCCreatureActionBase@@XZ (non-virtual, non-const); now
//   the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CCreatureAction_UseSwitch; a local class over its RTTI direct base CCreatureActionBase
//   engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_UseSwitch : public CCreatureActionBase
{
public:
    CCreatureAction_UseSwitch(const CCreatureAction_UseSwitch& other);   // 0x007EE160
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_UseSwitch@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB0 - sizeof(CCreatureActionBase)];   // CCreatureAction_UseSwitch's own members (not modelled: the copy constructor is out of line); sizeof 0xB0
};

CCreatureActionBase* CCreatureAction_UseSwitch::Clone() const
{
    return new CCreatureAction_UseSwitch(*this);
}
