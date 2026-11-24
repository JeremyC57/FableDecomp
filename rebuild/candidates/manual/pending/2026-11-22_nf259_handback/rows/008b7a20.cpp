// CCreatureAction_SummonCreatures::Initialise() -- play SUMMON_CREATURES, run the combat-action Initialise, spawn the summon def's effect at the summoner, then have its AI-summon component summon the creatures at the live target (SpawnOnTarget) or at the default spot
// Leaf: ?Initialise@CCreatureAction_SummonCreatures@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B7A20, length 240 bytes (plain `ret`, virtual; vftable 0x0127ADDC slot 12).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetCombatAnimation(const
// CCharString&) 0x008574D0, CCombatActionBase::Initialise 0x00857520 (direct base call), CBaseIntelligentPointer::GetPItem
// 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (inline GetTC 0xFA = CTCAISummon, bit 26 of the present-bits dword
// +0x3C), CThing::GetPos 0x004C73D0, CTCDParticleEmitter::Create(long, const C3DVector&, bool) 0x006E0880,
// CTCAISummon::SummonCreatures(const C3DVector*) 0x007FE040 (result dropped).
// Layout: CTCAISummon PSummonDef +0x0C (PDB); the effect read is CSummonDef +0x44 = SummonEffect (engine/CSummonDef.h; the def layout
// is the retail one: landed 0x007FDE00 reads TimeBetweenSummons at the header's +0x40); SpawnOnTarget +0x114; PTarget +0xA8.
// Shape: the summon component pointer is zero-initialised and filled by the unchecked out-parameter GetTC (the absent path dereferences
// null, as retail does); the two SummonCreatures calls are not merged.
// NF259 (2026-11-22, q12731, nf257 port debt): moved off the row-local CCombatActionBase / CCreatureAction_SummonCreatures / CSummonDef /
// CTCDParticleEmitter models onto the shared headers (the L72 prelude of the landed class rows 0x008B83C0 ..., engine/CSummonDef.h,
// engine/CTCDParticleEmitter.h CREATE); CTCAISummon has no shared header (none under rebuild/include) and keeps the HEAD row's local
// model (the landed sibling 0x007FDE00 models it locally too). The HEAD local named the +0x44 member SummoneeEffect; the header's
// SummonEffect is the member at that offset.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
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
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_PRIORITYCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CCREATUREACTION_SUMMONCREATURES_OVERRIDES
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE
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
#include "engine/CCreatureAction_SummonCreatures.h"
#include "engine/CSummonDef.h"
#include "engine/CTCDParticleEmitter.h"

class CTCAISummon { public:
    unsigned char _pad0[0x0c];
    CSummonDef*   PSummonDef;                   // +0x0c CDefPointer<const CSummonDef>
    long SummonCreatures(const C3DVector* position);   // 0x007FE040
};

void CCreatureAction_SummonCreatures::Initialise()
{
    {
        CCharString name("SUMMON_CREATURES", -1);
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    CTCAISummon* summon = 0;
    PCreature.Get()->GetTC(TCI_AI_SUMMON, &summon);
    long effect = summon->PSummonDef->SummonEffect;
    if (effect > 0)
        CTCDParticleEmitter::Create(effect, *(&PCreature.Get()->GetPos()), false);
    if (SpawnOnTarget && PTarget.Get() && !PTarget.Get()->IsDead())
        summon->SummonCreatures(&PTarget.Get()->GetPos());
    else
        summon->SummonCreatures(0);
}
