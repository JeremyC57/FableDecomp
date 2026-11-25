// CCreatureAction_GetItemOutOfPocket::Clone() const -- `return new CCreatureAction_GetItemOutOfPocket(*this);` with the implicit copy constructor inlined (CCreatureActionBase copy constructor, vftable store, own members)
// Leaf: ?Clone@CCreatureAction_GetItemOutOfPocket@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00946070, 59 bytes: operator new(0xAC) 0x00BFEA1A, null test, CCreatureActionBase(const CCreatureActionBase&) 0x0062EBC0, C's vftable store, then 1 member copy;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_GetItemOutOfPocket@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; base copy constructor ??0CCreatureActionBase@@QAE@ABV0@@Z 0x0062EBC0 (Ego_r/FableWin; MELEE_COPYCTOR / COPYCTOR
//   opt-in).
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_GetItemOutOfPocket@@QAEPAUCBase_0062ebc0@@XZ (non-virtual, non-const,
//   over a made-up base model); now the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CCreatureAction_GetItemOutOfPocket (header debt); a local class over its RTTI direct base
//   CCreatureActionBase (engine/CCreatureActionBase.h).
// Members: Ego_r.pdb names at retail offsets (PDB offset - 0xC: the base shrinks from 0xB4 to 0xA8 with the retail STLport containers);
//   each width matches retail's copy.
#define FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_GetItemOutOfPocket : public CCreatureActionBase
{
public:
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_GetItemOutOfPocket@@UAEXXZ (Ego_r + FableWin name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    long ItemDefIndex;                        // +0xA8 (Ego_r.pdb long ItemDefIndex at +0xB4)
};

CCreatureActionBase* CCreatureAction_GetItemOutOfPocket::Clone() const
{
    return new CCreatureAction_GetItemOutOfPocket(*this);
}
