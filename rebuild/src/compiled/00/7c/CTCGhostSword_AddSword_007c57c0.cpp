// CTCGhostSword::AddSword
// Leaf: ?AddSword@CTCGhostSword@@AAEXABVC3DVector@@@Z
// Flags:
#define FABLE_RETAIL_GHOST_SWORD_ADD_PASS27
#define FABLE_RETAIL_STLP_VECTOR_LONG_PUSHBACK_OWNER_PASS26
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_BULLETTIMEOVERRIDE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGGAMEOBJECT_COMBOMULTIPLIER
#define FABLE_RETAIL_CTHINGCREATUREBASE_FLAGSETTERS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGHOSTSWORD
#define FABLE_RETAIL_CTCGHOSTSWORD_FRAME
#define FABLE_RETAIL_CTHINGAICREATUREINIT_CTOR_BODY
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_CTCENEMY_ADDALLYTHING
#define FABLE_RETAIL_CTCREGIONFOLLOWER
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY

#define FABLE_RETAIL_CINTELLIGENTPOINTER_AICREATURE_SIZE_PASS27
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
// Fidelity: scaffolding, not retail. Own the 28-byte deleting destructor
// at 0x004D6931 in size mode. The constructor and method below use the
// original speed mode restored by optimize("", on).
#if defined(_MSC_VER)
#pragma optimize("s", on)
#endif
#include "engine/CIntelligentPointer_CreatureSizes.h"

#include "engine/CThingAICreature.h"
inline void CIntelligentPointer<CThingAICreature>::SetPointer(const CThingAICreature* pointer) {
    SetPItem(const_cast<CBaseObject*>(static_cast<const CBaseObject*>(pointer)));
}
inline CThingAICreature* CIntelligentPointer<CThingAICreature>::GetPointer() const {
    return static_cast<CThingAICreature*>(GetPItem());
}

#if defined(_MSC_VER)
#pragma optimize("", on)
#endif
inline CIntelligentPointer<CThingAICreature>::CIntelligentPointer(CThingAICreature* pointer) { SetPointer(pointer); }

#include "engine/CThingAICreatureInit.h"
#include "engine/CTCGhostSword.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCEnemy.h"
#include "engine/CFaction.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCRegionFollower.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/_quarantine/CSpecialAbilitiesGhostSwordDef.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCEffectOnDie.h"
#include "engine/CMainGameComponent.h"
#include "engine/ThingAlive.h"

void CTCGhostSword::AddSword(const C3DVector& createPos)
{
    CThingCreatureBase* pcreature = GFDebugPDynamicCast(&GetParentThing(), (CThingCreatureBase*)0);

    CTCSoundPlayer* ptc_sound_player;
    if (pcreature->GetTC(&ptc_sound_player))
        ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_GHOSTLY_SWORD_APPEAR", true, false, false, -1);

    long def_num = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("GHOSTLY_SWORD");
    CThingAICreatureInit init;
    CIntelligentPointer<CThingAICreature> minion(CThingAICreature::Create(def_num, createPos, pcreature->GetPlayerNumber(), init));
    minion->SetScriptName("HeroSummonnedCreature");

    CTCEnemy* ptc_enemy = 0;
    CTCEnemy* ptc_enemy_parent = 0;
    minion->GetTC(&ptc_enemy);
    pcreature->GetTC(&ptc_enemy_parent);
    ptc_enemy->SetFaction(ptc_enemy_parent->GetPFaction()->GetName());
    ptc_enemy->AddAllyThing(*pcreature);

    CTCCreatureNavigation* ptc_navigation;
    if (minion->GetTC(&ptc_navigation))
        ptc_navigation->SetAsAbleToMoveThroughSolidObjects(true);

    CTCRegionFollower* ptc_region_follower;
    if (pcreature->GetTC(&ptc_region_follower))
        ptc_region_follower->AddFollower(*minion);
    minion->SetAsFollowingHeroThroughTeleporters(true);

    if (!minion->HasTC(TCI_PARTICLE_ATTACHMENT_TRACKER))
        minion->AddTC("CTCParticleAttachmentTracker", false, 0);

    CDefPointer<const CSpecialEffectsDef> pspecial_effects;
    long effect = 0;
    if (PDef->GetSubDef(&pspecial_effects)) {
        if (pspecial_effects->GetEffectFromString("GHOST_SWORD_APPEARING", effect)) {
            CThing* thing = CTCSpecialAbilities::CreateParticleEffect(effect, createPos);
        }
        if (pspecial_effects->GetEffectFromString("GHOST_SWORD_ALIVE_EFFECT", effect)) {
            CThing* sword_effect = CTCSpecialAbilities::CreateParticleEffect(effect, createPos);
            if (IS_THING_ALIVE(sword_effect)) {
                CTCDParticleEmitter* pemitter;
                sword_effect->GetTC(&pemitter);
                if (pemitter)
                    pemitter->AttachToThing(*minion, 10, "EMITTER_BASE", 0, 0.0f);
            }
            MinionEffects.push_back(CIntelligentPointer<CThing>(sword_effect));
        }

        CTCEffectOnDie* ptc_ghost_fx = GFDebugPDynamicCast(minion->AddTC("CTCEffectOnDie", true, 0), (CTCEffectOnDie*)0);
        if (pspecial_effects->GetEffectFromString("GHOST_SWORD_ON_DIE_EFFECT", effect))
            ptc_ghost_fx->SetEffect(effect);
        ptc_ghost_fx->SetTimer((long)(CMainGameComponent::GetConstantFPS() * PDef->TimeOfSummon[SpellLevel]));
        ptc_ghost_fx->SetSoundEffect("HERO_ABILITY_GHOSTLY_SWORD_DISAPPEAR");
        ptc_ghost_fx->SetAlphaFadeOutTimer((long)(CMainGameComponent::GetConstantFPS() * PDef->FadeOutTime));
    }

    minion->SetLeaveDeadBodyOverride(false);
    minion->SetLeaveExperienceOrbsOverride(false);
    minion->SetIsAffectedByBulletTimeOverride(false);
    minion->SetAddToComboMultiplierOnHit(false);

    Minions.push_back(minion);
    CurFadeInTimer.push_back(FadeInTimer);
    pcreature->MakeNoise("SIMPLE_NOISE");
}