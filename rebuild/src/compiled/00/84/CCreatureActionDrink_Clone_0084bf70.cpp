// CCreatureAction_Drink::Clone() const -- `return new CCreatureAction_Drink(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_Drink@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084BF70, 34 bytes: operator new(0xB8) 0x00BFEA1A, null test, then the copy constructor 0x0084BFA0 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_Drink@@UBEPAVCCreatureActionBase@@XZ: RTTI placement + Ego_r/FableWin;
//   callee ??0CCreatureAction_Drink@@QAE@ABV0@@Z: Ego_r/FableWin name set; retail 0x0084BFA0 stores CCreatureAction_Drink's vftable
//   0x0127473C first; the landed row at 0x0084BFA0 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_Drink@@QAEPAVCCreatureActionBase@@XZ (non-virtual, non-const); now the
//   real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CCreatureAction_Drink; a local class over its RTTI direct base CCreatureActionBase
//   engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_Drink : public CCreatureActionBase
{
public:
    CCreatureAction_Drink(const CCreatureAction_Drink& other);   // 0x0084BFA0
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_Drink@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB8 - sizeof(CCreatureActionBase)];   // CCreatureAction_Drink's own members (not modelled: the copy constructor is out of line); sizeof 0xB8
};

CCreatureActionBase* CCreatureAction_Drink::Clone() const
{
    return new CCreatureAction_Drink(*this);
}
