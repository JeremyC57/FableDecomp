// CTCCombat::AttemptToStartParticle(CThing* weapon) -- start the weapon's flourish particles: for each start/end helper pair of the weapon def's flourish set, create the effect and the blend-to effect at the weapon, and when both live blend the first emitter towards the second by the helper distance, attach it to the weapon and kill the second
// Grade: DIFFER(993v973) 72 hunks -- weapon arg in ebx v ebp (allocation), the FlourishParticles[i] = CIntelligentPointer temporary built differently (ours stores 0 + ctor; retail ctor 0x00A01B10 then vptr 0x01238C6C then SetPItem), loop counter not mirrored to [esp+20h], one fsub pair order in the distance, AttachToThing push order
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
namespace NParticleEngine {
class CParticleEmitter : public CBaseObject {          // no engine header: local model of the members this row touches
public:
    float GetApproximateTotalSplineLength() const;      // 0x00AED050
    void InterpolateTo(const CParticleEmitter& other, float amount);   // 0x00AED600 ?InterpolateTo@CParticleEmitter@NParticleEngine@@QAEXABV12@M@Z
    unsigned char _pad_base[0x41 - sizeof(CBaseObject)];
    unsigned char Flags41;                              // +0x41 (bit 1 set on the blend target)
    unsigned char _pad42[2];
    unsigned char Flags44;                              // +0x44 (bit 4 set on the blending emitter)
};
}
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCCombat.h"

void CTCCombat::AttemptToStartParticle(CThing* weapon)
{
    CDefPointer<const CWeaponDef> weapon_def;
    if (weapon->GetPBaseDef()->GetSubDef(&weapon_def)) {
        const CObjectAugmentationParticleSet& set = weapon_def->FlourishParticles;
        CurFlourishTimer = (long)(CMainGameComponent::GetConstantFPS() * set.PeriodSecs);
        long count = set.StartPoint.size();
        FlourishParticles.resize(count);
        for (long i = 0; i < count; ++i) {
            long blend_effect = set.ParticleEffectToBlendTo;
            long effect = set.ParticleEffect;
            CCharString start_name = set.StartPoint[i].GetString();
            CCharString end_name = set.EndPoint[i].GetString();
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
                if (FlourishParticles[i].Get() && !FlourishParticles[i].Get()->IsDead() && blend_thing && !blend_thing->IsDead()) {
                    CTCDParticleEmitter* emitter = 0;
                    CTCDParticleEmitter* blend_emitter = 0;
                    FlourishParticles[i].Get()->GetTC(&emitter);
                    blend_thing->GetTC(&blend_emitter);
                    NParticleEngine::CParticleEmitter* particles = emitter->GetPEmitter();
                    NParticleEngine::CParticleEmitter* blend_particles = blend_emitter->GetPEmitter();
                    blend_particles->Flags41 |= 2;
                    float distance = (start_pos - end_pos).GetMagnitude();
                    float blend_length = blend_particles->GetApproximateTotalSplineLength();
                    float length = particles->GetApproximateTotalSplineLength();
                    float range = blend_length - length;
                    particles->InterpolateTo(*blend_particles, (distance - length) / std::max(range, 0.0001f) * 2.0f);
                    particles->Flags44 |= 0x10;
                    emitter->AttachToThing(*weapon, 10, start_name, 0, 0.0f);
                    blend_thing->Kill(true);
                }
            }
        }
    }
}
