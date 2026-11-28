// Leaf: ?Initialise@CCreatureAction_BriarRoseConcentrateInto@@UAEXXZ
// Flags:
#define FABLE_RETAIL_BRIAR_ROSE_OLD_BINDER_PASS30
#define FABLE_RETAIL_CCREATUREACTION_BRIARROSECONCENTRATEINTO
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SETCOMBATANIMATION
#define FABLE_RETAIL_KRAKEN_ROAR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_NCREATUREMODE_CMODEBRIARROSECONCENTRATE_CTOR
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCCOMBAT_ISTOBLOCK_PASS30
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/NCreatureMode_CModeBriarRoseConcentrate.h"
#include "engine/ThingAlive.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_BriarRoseConcentrateInto.h"
#include "engine/CTCDParticleEmitter.h"

void CCreatureAction_BriarRoseConcentrateInto::Initialise()
{
    CTCCreatureModeManager* modes = 0;
    PCreature->UnsheathePrimaryWeapon();
    SetCombatAnimation("POWER_UP_INTO");
    CCombatActionBase::Initialise();
    if (PCreature->GetTC(&modes) && IS_THING_ALIVE(PEnemy))
        modes->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>(
            new NCreatureMode::CModeBriarRoseConcentrate(*PCreature,*PEnemy,Imposter)));
    CDefPointer<const CSpecialEffectsDef> effects;
    long effectType = 0;
    bool found = PCreature->GetPBaseDef()->GetSubDef(&effects) &&
                 effects->GetEffectFromString("TELEPORT_INTO",effectType);
    if (found)
        CTCDParticleEmitter::Create(effectType,PCreature->GetPos(),false);
}
