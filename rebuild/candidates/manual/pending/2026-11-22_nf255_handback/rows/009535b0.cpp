// CCreatureAction_DragonFireBeamInto::CCreatureAction_DragonFireBeamInto(CThingCreatureBase& creature, CThing& target, float time) --
//   combat action on a target with the SWORD_SWING sound pair (priority 250); the beam timer comes from the argument
// Leaf: ??0CCreatureAction_DragonFireBeamInto@@QAE@AAVCThingCreatureBase@@AAVCThing@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009535B0, length 97 bytes, `ret 0Ch` (three arguments).
// Identity: the stored vtable 0x01293E54 is CCreatureAction_DragonFireBeamInto (rtti-vtables.tsv). Ego_r.pdb gives the class one float,
//   Timer (PDB +0x12c, retail +0x114: the retail CCombatActionBase is 0x114 bytes).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::GetSoundPair (__fastcall) @ 0x008561A0,
//          CCombatActionBase::CCombatActionBase(CThingCreatureBase&, CThing&, long, const CSoundPair&) @ 0x00857F30,
//          CCharString::~CCharString @ 0x0099EAE0.
// Shape: the "SWORD_SWING" literal converts implicitly to a CCharString temporary (re-addressed with `lea`), the
//   CSoundPair (user constructor -> returned through the hidden pointer in ecx) lands in a frame temporary that is
//   passed by reference to the base constructor; the string dies after the base call, then the vtable store and the
//   Timer initialiser (a float copied through edx from the third argument slot).
// Names (agentNF255, 2026-11-22, q12720 / CODEXP27 L92; NF250's draft): was ??0CCreatureAction_DragonFireBeamInto@@QAE@PAVCThingCreatureBase@@PAVCThing@@M@Z
//   from row-local models. Ego_r.pdb 0x00977F90 and FableWin.pdb 0x02BE2090 both spell
//   ??0CCreatureAction_DragonFireBeamInto@@QAE@AAVCThingCreatureBase@@AAVCThing@@M@Z; the landed caller 0x00951FB0
//   (CAIStateGroup_JackDragon::UpdateHover) already binds it.
// Vtable: this object emits ??_7CCreatureAction_DragonFireBeamInto@@6B@ (81 slots, retail 0x01293E54). The prelude is the CCombatActionBase
//   MELEE branch with the override opt-ins of the landed 0x00955620 (MAZE_DEFENSIVE_MELEE_PASS27, FRAMEUPDATE, INITIALISE, FINISHACTION,
//   ONANIMATIONLOADED, MELEE_OWN_VIRTUALS) plus the class's own Initialise override (DRAGONFIREBEAMINTO_INITIALISE), so slot for slot the
//   table names the retail owner of each target: 0 / 11 / 12 this class, 3, 4, 6-8, 10, 13-15, 17-23, 27-29, 43, 44, 51, 53, 54, 56, 57,
//   59-62, 66, 68, 70-80 CCombatActionBase, the rest CCreatureActionBase. The same prelude text is in the Clone (0x00953CF0) and
//   deleting-destructor (0x009539C0) rows, so the three COMDAT copies are one.
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
#define FABLE_RETAIL_JACKDRAGON_HOVER_PASS27
#define FABLE_RETAIL_CCREATUREACTION_DRAGONFIREBEAMINTO_INITIALISE
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
#include "engine/CCreatureAction_DragonFireBeamInto.h"

CCreatureAction_DragonFireBeamInto::CCreatureAction_DragonFireBeamInto(CThingCreatureBase& creature, CThing& target, float time)
    : CCombatActionBase(creature, target, 250, GetSoundPair(creature, "SWORD_SWING"))
    , Timer(time)
{
}
