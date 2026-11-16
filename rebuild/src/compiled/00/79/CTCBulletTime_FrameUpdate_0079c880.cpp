// 0x0079C880 CTCBulletTime::FrameUpdate() -- refresh screen emitters while active, remove expired bullet time, and track/fade the hit-target blur.
// Leaf: ?FrameUpdate@CTCBulletTime@@UAEXXZ
// Retail: 0x0079C880, 933 bytes, thiscall.
// Flags: none
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLEEMITTER_KILL
#define FABLE_RETAIL_CPARTICLEEMITTER_INTERPOLATE
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_CREATE2D
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_BULLETTIME_PASS23
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_DEFAULT_IDENTITY
#define FABLE_RETAIL_CTCBULLETTIME
#define FABLE_RETAIL_CTCBULLETTIME_MEMBERS
#define FABLE_RETAIL_CTCBULLETTIME_PLAYEFFECT
#define FABLE_RETAIL_CTCBULLETTIME_FRAME_PASS23
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERPOLATOR
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_BULLETTIME_PASS23
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CSpecialAbilitiesBulletTimeDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CParticleEmitter.h"
#include "engine/CParticleEmitterManager.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CEnvironmentSpellEffectInterpolator.h"
#include "engine/CTCBulletTime.h"

float FABLE_FASTCALL GFVXToAX(float);
float FABLE_FASTCALL GFVYToAY(float);
extern double VIRTUAL_SCREEN_WIDTH,VIRTUAL_SCREEN_HEIGHT;

void CTCBulletTime::FrameUpdate()
{
    CThingCreatureBase* pcreature = GFDebugPDynamicCast(&GetParentThing(),(CThingCreatureBase*)0);
    if (!Paused) {
        --BulletTimeRemainingTicks;
        if (!m_tpLoopingEffect.Get() || m_tpLoopingEffect.Get()->IsDead()) {
            CDefPointer<const CSpecialEffectsDef> pspecial_effects;
            long effect = 0;
            if (PDef->GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString("SLOWTIME", effect)) {
                m_tpLoopingEffect = NParticleEngine::GetParticleEmitterManager().Create2DEmitter(effect,
                    C2DVector(GFVXToAX((float)VIRTUAL_SCREEN_WIDTH) * 0.5f, GFVYToAY((float)VIRTUAL_SCREEN_HEIGHT) * 0.5f), (EEngine2DLayer)0x52, true,
                    CRightHandedSet());
                if (m_tpLoopingEffect.Get() && !m_tpLoopingEffect.Get()->IsDead())
                    m_tpLoopingEffect.Get()->SetAlwaysUpdate(true);
            }
        }
        if (BulletTimeRemainingTicks == 0) {
            CDefPointer<const CSpecialEffectsDef> pspecial_effects;
            long effect = 0;
            if (PDef->GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString("END", effect)) {
                NParticleEngine::CParticleEmitter* pemitter = NParticleEngine::GetParticleEmitterManager().Create2DEmitter(effect,
                    C2DVector(GFVXToAX((float)VIRTUAL_SCREEN_WIDTH) * 0.5f, GFVYToAY((float)VIRTUAL_SCREEN_HEIGHT) * 0.5f), (EEngine2DLayer)0x52, true,
                    CRightHandedSet());
                if (pemitter && !pemitter->IsDead())
                    pemitter->SetAlwaysUpdate(true);
            }
            Pausable = false;
            pcreature->RemoveTC(TCI_BULLET_TIME);
        }
    }
    if (HitCreatureBlur.Get() && !HitCreatureBlur.Get()->IsDead()) {
        CEnvironmentSpellEffectInterpolator& sei = GetWorld().GetEnvironment().GetSpellEffectInterpolator();
        sei.RadialBlurSetCenterWorldPos(RadialBlurKey, HitCreatureBlur.Get()->GetPos());
        if (sei.GetRadialBlurParam(RadialBlurKey).Fade == 0.0f)
            HitCreatureBlur = CIntelligentPointer<CThing>(0);
    }
}