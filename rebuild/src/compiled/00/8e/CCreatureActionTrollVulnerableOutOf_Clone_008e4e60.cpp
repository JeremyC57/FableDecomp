// CCreatureAction_TrollVulnerableOutOf::Clone() const -- `return new CCreatureAction_TrollVulnerableOutOf(*this);` with the implicit copy constructor inlined (CCombatActionBase copy constructor, vftable store)
// Leaf: ?Clone@CCreatureAction_TrollVulnerableOutOf@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E4E60, 47 bytes: operator new(0x114) 0x00BFEA1A, null test, CCombatActionBase(const CCombatActionBase&) 0x0062F3B0, C's vftable store;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_TrollVulnerableOutOf@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; base copy constructor ??0CCombatActionBase@@QAE@ABV0@@Z 0x0062F3B0 (Ego_r/FableWin; MELEE_COPYCTOR / COPYCTOR
//   opt-in).
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_TrollVulnerableOutOf@@QAEPAVCCombatActionBase@@XZ (non-virtual,
//   non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CCreatureAction_TrollVulnerableOutOf (header debt); a local class over its RTTI direct base
//   CCombatActionBase (engine/CCombatActionBase.h).
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

class CCreatureAction_TrollVulnerableOutOf : public CCombatActionBase
{
public:
public:
    virtual CCreatureActionBase* Clone() const;
};

CCreatureActionBase* CCreatureAction_TrollVulnerableOutOf::Clone() const
{
    return new CCreatureAction_TrollVulnerableOutOf(*this);
}
