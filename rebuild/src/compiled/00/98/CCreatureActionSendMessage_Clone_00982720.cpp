// CCreatureAction_SendMessage::Clone() const -- `return new CCreatureAction_SendMessage(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_SendMessage@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00982720, 34 bytes: operator new(0xC4) 0x00BFEA1A, null test, then the copy constructor 0x00982750 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_SendMessage@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_SendMessage@@QAE@ABV0@@Z: Ego_r/FableWin name set; retail 0x00982750 stores
//   CCreatureAction_SendMessage's vftable 0x0129947C first; the landed row at 0x00982750 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_SendMessage@@QAEPAVCCreatureActionBase@@XZ (non-virtual, non-const);
//   now the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CCreatureAction_SendMessage; a local class over its RTTI direct base CCreatureActionBase
//   engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_SendMessage : public CCreatureActionBase
{
public:
    CCreatureAction_SendMessage(const CCreatureAction_SendMessage& other);   // 0x00982750
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_SendMessage@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xC4 - sizeof(CCreatureActionBase)];   // CCreatureAction_SendMessage's own members (not modelled: the copy constructor is out of line); sizeof 0xC4
};

CCreatureActionBase* CCreatureAction_SendMessage::Clone() const
{
    return new CCreatureAction_SendMessage(*this);
}
