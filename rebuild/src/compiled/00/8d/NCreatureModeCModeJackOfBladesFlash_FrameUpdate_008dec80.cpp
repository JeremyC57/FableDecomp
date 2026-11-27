// 008dec80 NCreatureMode::CModeJackOfBladesFlash::FrameUpdate
// Leaf: ?FrameUpdate@CModeJackOfBladesFlash@NCreatureMode@@UAEXXZ
// Flags:
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#include "fable_assert.h"
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_XYZ
#define FABLE_RETAIL_CSPOTLIGHTDEF
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_GFANGLES_SIGNED_ROUNDING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_JACK_FLASH_FRAME_PASS29
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_LIGHTINGFLASH
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTHINGMARKER_CREATE
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#define FABLE_RETAIL_CTCBOSS_ISREADYFORPHASECHANGE
#define FABLE_RETAIL_CTCSPOTLIGHT_BASES
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CBASEOBJECT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefClassBase.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/NCreatureMode.h"
#include "engine/GFAngles.h"
#include "engine/GFLimit.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingMarker.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#include "engine/CTCBoss.h"
#include "engine/CTCSpotLight.h"
#include "engine/CEnvironment.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CWorld.h"
#include "engine/NCreatureMode_CModeJackOfBladesFlash.h"
#include "engine/ThingAlive.h"
template<> bool CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>* out) const
{
    const CSubDefInfo* index = GetPSubDefInfo(CSpecialEffectsDef::StatGetTypeName());
    if (index) {
        CDefPointer<const CSpecialEffectsDef> result(PDefManager->GetPDefFromGlobalIndex(index->DefIndex));
        *out=result;
        return true;
    }
    return false;
}
namespace { extern long JACK_FLASH_DURATION, JACK_FADE_UP_TIME, JACK_GLOW_START_FRAME; }
namespace NCreatureMode {
void CModeJackOfBladesFlash::FrameUpdate()
{
    CTCCreatureModeManager::CModeBase::FrameUpdate();
    long glow_def_num = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("ENVIRONMENT_JACK_OF_BLADES_GLOW");
    --Timer;
    if (Timer < JACK_FLASH_DURATION)
        Flash();
    if (Timer == JACK_GLOW_START_FRAME) {
        if (IS_THING_ALIVE(PEmitter))
            PEmitter->Kill(true);
        GetWorld().GetEnvironment().TransitionToThemeAllChannels(3, glow_def_num, (float)(JACK_FADE_UP_TIME / 30.0f));
        CDefPointer<const CSpecialEffectsDef> pspecial_fx;
        if (GetParentThing().PeekBaseDef().GetSubDef(&pspecial_fx)) {
            long fx_index;
            if (pspecial_fx->GetEffectFromString("FLASH_SWORD_GLOW", fx_index)) {
                C3DVector weapon_pos = PCreature->GetPos();
                CRightHandedSet rhs;
                if (!(PCreature->GetTCGraphicAppearance()->GetDummyObject("WEAPON_FOCUS_01", 0, 0, weapon_pos, rhs))) { FABLE_RETAIL_ASSERT(false); }
                CThing* pemitter = CTCDParticleEmitter::Create(fx_index, weapon_pos, false);
                if (IS_THING_ALIVE(pemitter))
                    PEmitter = pemitter;
                long def_num = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("MARKER_SPOT_LIGHT_JACK_OF_BLADES");
                CThing* plight_marker = CThingMarker::Create(def_num, weapon_pos, "");
                if (IS_THING_ALIVE(plight_marker))
                    PSpotLight = plight_marker;
            }
        }
    } else if (Timer == 0) {
        long fade_def_num = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("ENVIRONMENT_JACK_OF_BLADES_FADE");
        GetWorld().GetEnvironment().TransitionToThemeAllChannels(3, fade_def_num, 0.75f);
        if (IS_THING_ALIVE(PEmitter))
            PEmitter->Kill(true);
        if (IS_THING_ALIVE(PSpotLight))
            PSpotLight->Kill(true);
        ExtinguishFlames();
        PCreature->SetCurrentAction(CCreatureAction_PlayCombatAnimation(*PCreature, "ST_STRIKE_LIGHT_OUTOF", true));
        CTCCreatureModeManager* tc_mode_manager;
        if (!(PCreature->GetTC(&tc_mode_manager))) { FABLE_RETAIL_ASSERT(false); }
        tc_mode_manager->RemoveMode(MODE_JACK_OF_BLADES_FLASH);
        return;
    }
    if (IS_THING_ALIVE(PSpotLight)) {
        C3DVector camera_pos = GetWorld().GetGameCamera()->PeekCurrentPos();
        C3DVector dir_to_camera = camera_pos - GetParentThing().GetPos();
        dir_to_camera.Normalise();
        C3DVector dir_to_camera_prime = dir_to_camera;
        dir_to_camera_prime.Z = 0.0f;
        dir_to_camera_prime.Normalise();
        PSpotLight->GetPhysicsTC()->ResetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), dir_to_camera_prime));
        CTCSpotLight* ptc_spot_light = 0;
        if (!(PSpotLight->GetTC(&ptc_spot_light))) { FABLE_RETAIL_ASSERT(false); }
        float cos_theta = dir_to_camera.Dot(C3DVector(0.0f, 0.0f, -1.0f));
        GFLimit(cos_theta, -1.0f, 1.0f);
        float theta = GFArcCos(cos_theta);
        ptc_spot_light->SetAngle(theta);
    }
    CTCBoss* pTCBoss = 0;
    if (!(PCreature->GetTC(&pTCBoss))) { FABLE_RETAIL_ASSERT(false); }
    if (pTCBoss->IsReadyForPhaseChange()) {
        CTCCreatureModeManager* tc_mode_manager = 0;
        if (!(PCreature->GetTC(&tc_mode_manager))) { FABLE_RETAIL_ASSERT(false); }
        tc_mode_manager->RemoveMode(MODE_JACK_OF_BLADES_FLASH);
    }
}
}