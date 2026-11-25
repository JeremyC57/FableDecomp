// CCreatureAction_CastWillHealLifeSpell::Clone() const -- `return new CCreatureAction_CastWillHealLifeSpell(*this);` with the implicit copy constructor inlined (CCombatActionBase copy constructor, vftable store, own members)
// Leaf: ?Clone@CCreatureAction_CastWillHealLifeSpell@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BE620, 59 bytes: operator new(0x118) 0x00BFEA1A, null test, CCombatActionBase(const CCombatActionBase&) 0x0062F3B0, C's vftable store, then 1 member copy;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_CastWillHealLifeSpell@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; base copy constructor ??0CCombatActionBase@@QAE@ABV0@@Z 0x0062F3B0 (Ego_r/FableWin; MELEE_COPYCTOR / COPYCTOR
//   opt-in).
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_CastWillHealLifeSpell@@QAEPAUCBase_0062f3b0@@XZ (non-virtual,
//   non-const, over a made-up base model); now the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CCreatureAction_CastWillHealLifeSpell (header debt); a local class over its RTTI direct base
//   CCombatActionBase (engine/CCombatActionBase.h).
// Members: Ego_r.pdb names at retail offsets (PDB offset - 0x18: the base shrinks from 0x12C to 0x114 with the retail STLport
//   containers); each width matches retail's copy.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_COPYCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CTCPhysicsBase.h"

class CCreatureAction_CastWillHealLifeSpell : public CCombatActionBase
{
public:
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_CastWillHealLifeSpell@@UAEXXZ (Ego_r + FableWin name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    long Level;                               // +0x114 (Ego_r.pdb long Level at +0x12C)
};

CCreatureActionBase* CCreatureAction_CastWillHealLifeSpell::Clone() const
{
    return new CCreatureAction_CastWillHealLifeSpell(*this);
}
