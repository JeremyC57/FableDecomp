// CTCCombat::AttemptToStartParticle(CThing* weapon) -- start the weapon's flourish particles: for each start/end helper pair of the weapon def's flourish set, create the effect and the blend-to effect at the weapon, and when both live blend the first emitter towards the second by the helper distance, attach it to the weapon and kill the second
// Leaf: ?AttemptToStartParticle@CTCCombat@@AAEXPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00735FD0, 973 bytes (`ret 4`).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_FLOURISH
#define FABLE_RETAIL_CTCCOMBAT_ABILITYLIST
#define FABLE_RETAIL_CTCCOMBAT_ATTEMPTTOSTARTPARTICLE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CWEAPONDEF_FLOURISHPARTICLES
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_GETPEMITTER_INLINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_CDEFSTRING_FORCEINLINE_GETSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLEEMITTER_INTERPOLATE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CRightHandedSet.h"
#include "engine/CObjectAugmentationParticleSet.h"
#include "engine/CWeaponDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CParticleEmitter.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCCombat.h"
#include "engine/ThingAlive.h"    // FableWin.pdb ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_NABV?$CIntelligentPointer@VCThing@@@@@Z / ..@PAVCThing@@@@: the element address is taken once for both GetPItem calls (0x0073619C..0x007361B3) and blend_thing has a frame home (+0x1C)
extern const float DELTA_NOTIONAL_ZERO;                                 // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB (1e-4f)

void CTCCombat::AttemptToStartParticle(CThing* weapon)
{
    CDefPointer<const CWeaponDef> weapon_def;
    if (weapon->GetPBaseDef()->GetSubDef(&weapon_def)) {
        CurFlourishTimer = (long)(CMainGameComponent::GetConstantFPS() * weapon_def->FlourishParticles.PeriodSecs);
        long count = weapon_def->FlourishParticles.StartPoint.size();
        FlourishParticles.resize(count);
        for (long i = 0; i < count; ++i) {
            long blend_effect = weapon_def->FlourishParticles.ParticleEffectToBlendTo;
            long effect = weapon_def->FlourishParticles.ParticleEffect;
            CCharString start_name = weapon_def->FlourishParticles.StartPoint[i].GetString();
            CCharString end_name = weapon_def->FlourishParticles.EndPoint[i].GetString();
            C3DVector start_pos;
            C3DVector end_pos;
            CRightHandedSet orientation;
            if (!weapon->GetTCGraphicAppearance()->GetHelperPoint(start_name, 0, 0, start_pos, 1.0f)
                && !weapon->GetTCGraphicAppearance()->GetDummyObject(start_name, 0, 0, start_pos, orientation, 1.0f))
                return;
            if (!weapon->GetTCGraphicAppearance()->GetHelperPoint(end_name, 0, 0, end_pos, 1.0f)
                && !weapon->GetTCGraphicAppearance()->GetDummyObject(end_name, 0, 0, end_pos, orientation, 1.0f))
                return;
            if (effect > 0) {
                FlourishParticles[i] = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect, weapon->GetPos(), true));
                CThing* blend_thing = CTCDParticleEmitter::Create(blend_effect, weapon->GetPos(), true);
                if (IS_THING_ALIVE(FlourishParticles[i]) && IS_THING_ALIVE(blend_thing)) {
                    CTCDParticleEmitter* emitter = 0;
                    CTCDParticleEmitter* blendEmitter = 0;
                    FlourishParticles[i].Get()->GetTC(&emitter);
                    blend_thing->GetTC(&blendEmitter);
                    NParticleEngine::CParticleEmitter* particles = emitter->GetPEmitter();
                    NParticleEngine::CParticleEmitter* blend_particles = blendEmitter->GetPEmitter();
                    blend_particles->Sleep();
                    float distance = (start_pos - end_pos).GetMagnitude();
                    float blend_length = blend_particles->GetApproximateTotalSplineLength();
                    float length = particles->GetApproximateTotalSplineLength();
                    float range = blend_length - length;
                    particles->InterpolateTo(*blend_particles, (distance - length) / std::max(range, DELTA_NOTIONAL_ZERO) * 2.0f);
                    particles->SetAlwaysUpdate(true);
                    emitter->AttachToThing(*weapon, 10, start_name, 0, 0.0f);
                    blend_thing->Kill(true);
                }
            }
        }
    }
}
