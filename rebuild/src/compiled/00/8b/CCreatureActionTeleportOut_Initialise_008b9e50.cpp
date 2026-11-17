// CCreatureAction_TeleportOut::Initialise() -- the combat-action Initialise, then (when the action creates effects) look up the creature definition's special-effects sub-def and, if it maps "TELEPORT_OUT" to an effect type, spawn that effect at the creature's position
// Leaf: ?Initialise@CCreatureAction_TeleportOut@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B9E50, length 208 bytes (plain `ret`, virtual).
// NF67 port (round 7): onto the shared headers -- engine/CCreatureAction_TeleportOut.h (FABLE_RETAIL_CCREATUREACTION_TELEPORT: TeleportPos
//   +0x114, CreateEffects +0x120), engine/CCombatActionBase.h (Initialise 0x00857520), engine/CCreatureActionBase.h (PCreature +0x08),
//   engine/CThing.h / CThingBaseDef.h (PeekBaseDef, the +0x70 word; GetPos 0x004C73D0), engine/CDefClassBase_GetSubDef.h
//   (GetSubDef<CSpecialEffectsDef> 0x00669580, body visible and out of line), engine/CSpecialEffectsDef.h (GetEffectFromString 0x00669D30)
//   and engine/CTCDParticleEmitter.h (static Create(long, const C3DVector&, bool) 0x006E0880).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCREATUREACTION_TELEPORT
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_TeleportOut.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CTCDParticleEmitter.h"

void CCreatureAction_TeleportOut::Initialise()
{
    CCombatActionBase::Initialise();
    if (CreateEffects) {
        CDefPointer<const CSpecialEffectsDef> pspecial_effects_def;
        long effect_type = 0;
        if (PCreature->PeekBaseDef().GetSubDef(&pspecial_effects_def)
            && pspecial_effects_def->GetEffectFromString("TELEPORT_OUT", effect_type))
            CTCDParticleEmitter::Create(effect_type, PCreature->GetPos(), false);
    }
}
