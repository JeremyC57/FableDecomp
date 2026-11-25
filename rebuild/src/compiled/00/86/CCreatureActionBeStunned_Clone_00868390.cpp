// CCreatureAction_BeStunned::Clone() const -- `return new CCreatureAction_BeStunned(*this);` with the implicit copy constructor inlined (CCreatureActionBase copy constructor, vftable store, own members)
// Leaf: ?Clone@CCreatureAction_BeStunned@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00868390, 59 bytes: operator new(0xAC) 0x00BFEA1A, null test, CCreatureActionBase(const CCreatureActionBase&) 0x0062EBC0, C's vftable store, then 1 member copy;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_BeStunned@@UBEPAVCCreatureActionBase@@XZ: RTTI placement + Ego_r/FableWin;
//   base copy constructor ??0CCreatureActionBase@@QAE@ABV0@@Z 0x0062EBC0 (Ego_r/FableWin; MELEE_COPYCTOR / COPYCTOR opt-in).
// Respell (NF425; NF427 retail override set): was the stand-in self ?Clone@CCreatureAction_BeStunned@@QAEPAUCBase_0062ebc0@@XZ (non-virtual, non-const, over a
//   made-up base model); now the real virtual member, so the RTTI-placed name is defined.
// Overrides (NF427): no engine/CCreatureAction_BeStunned.h exists, so the local class declares the class's retail override set: every
//   slot of retail vftable 0x01276E24 that differs from CCreatureActionBase's (evidence/slotdiff.tsv), each RTTI-placed and in a PDB
//   name set; the emitted ??_7CCreatureAction_BeStunned@@6B@ names all 70 of retail's slots (0 DIFF, 0 WRONG).
// Header: no engine header defines CCreatureAction_BeStunned (header debt); a local class over its RTTI direct base CCreatureActionBase
//   (engine/CCreatureActionBase.h).
// Members: Ego_r.pdb names at retail offsets (PDB offset - 0xC: the base shrinks from 0xB4 to 0xA8 with the retail STLport containers);
//   each width matches retail's copy.
#define FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureActionBase;
class CCreatureAction_BeStunned : public CCreatureActionBase
{
public:
    virtual void PerformAction();                                     // slot 3 ?PerformAction@CCreatureAction_BeStunned@@UAEXXZ (fablewin)
    virtual long GetAnimDelayTime() const;                            // slot 6 ?GetAnimDelayTime@CCreatureAction_BeStunned@@UBEJXZ (fablewin)
    virtual long GetAnimationTransitionInTime() const;                // slot 8 ?GetAnimationTransitionInTime@CCreatureAction_BeStunned@@UBEJXZ (fablewin)
    virtual CCreatureActionBase* Clone() const;                       // slot 11 ?Clone@CCreatureAction_BeStunned@@UBEPAVCCreatureActionBase@@XZ (egor+fablewin)
    virtual void Initialise();                                        // slot 12 ?Initialise@CCreatureAction_BeStunned@@UAEXXZ (egor+fablewin)
    virtual void OnAnimationLoaded();                                 // slot 68 ?OnAnimationLoaded@CCreatureAction_BeStunned@@UAEXXZ (egor+fablewin)
public:
    bool Flip;                                // +0xA8 (Ego_r.pdb bool Flip at +0xB4)
};

CCreatureActionBase* CCreatureAction_BeStunned::Clone() const
{
    return new CCreatureAction_BeStunned(*this);
}
