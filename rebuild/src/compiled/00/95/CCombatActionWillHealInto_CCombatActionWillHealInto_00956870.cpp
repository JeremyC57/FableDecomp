// CCombatAction_WillHealInto::CCombatAction_WillHealInto(CThingCreatureBase& creature, long spell_level) -- base
//   CCombatActionBase(creature, 150, GetSoundPair(creature, "SWORD_SWING")), SpellLevel at +0x114
// Leaf: ??0CCombatAction_WillHealInto@@QAE@AAVCThingCreatureBase@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00956870, length 90 bytes (+ int3 padding). Vtable 0x012948AC.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("SWORD_SWING" @ 0x0127AC80, -1),
// CCombatActionBase::GetSoundPair(const CThingCreatureBase&, const CCharString&) @ 0x008561A0 (__fastcall: CSoundPair returned through the
// hidden pointer in ecx, the creature in edx, the string reference pushed), CCombatActionBase::CCombatActionBase
// (CThingCreatureBase&, long, const CSoundPair&) @ 0x00858030, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the string argument is a CONVERSION temporary (the literal converts through CCharString(const char*, long = -1)), alive for
// the whole initialiser expression (destroyed after the base call); the sound pair result is built at [esp+8] and its address is pushed
// as the reference argument, then the vtable store and the member store from the second argument.
// Names (agentNF250 draft, agentNF257 2026-11-22, q12723 / CODEXP27 L100): was ??0CCombatAction_WillHealInto@@QAE@PAVCThingCreatureBase@@J@Z
//   from row-local models. Ego_r.pdb 0x0097B450 and FableWin.pdb 0x02BF9A10 both spell ??0CCombatAction_WillHealInto@@QAE@AAVCThingCreatureBase@@J@Z;
//   the landed caller 0x00955620 already binds it. Now on the shared engine/CCombatAction_WillHealInto.h (SpellLevel, size 0x118) and
//   engine/CCombatActionBase.h.
// Vtable: this object emits ??_7CCombatAction_WillHealInto@@6B@ (81 slots, retail 0x012948AC). The prelude is the CCombatActionBase
//   MELEE branch with the override opt-ins of the landed 0x00955620 (MAZE_DEFENSIVE_MELEE_PASS27, FRAMEUPDATE, INITIALISE, FINISHACTION,
//   ONANIMATIONLOADED, MELEE_OWN_VIRTUALS) plus the class's own overrides (WILLHEALINTO_OVERRIDES: FinishAction, Initialise, Clone), so each
//   slot names the retail owner of its target (0 / 4 / 11 / 12 this class). The same prelude text is in the three rows of the class that
//   emit the table (constructor 0x00956870, Clone 0x00956A20, deleting destructor 0x00956940), so the COMDAT copies are one.
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
#define FABLE_RETAIL_CCOMBATACTION_WILLHEALINTO_OVERRIDES
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
#include "engine/CCombatAction_WillHealInto.h"

CCombatAction_WillHealInto::CCombatAction_WillHealInto(CThingCreatureBase& creature, long spell_level)
    : CCombatActionBase(creature, 150, GetSoundPair(creature, "SWORD_SWING"))
    , SpellLevel(spell_level)
{
}
