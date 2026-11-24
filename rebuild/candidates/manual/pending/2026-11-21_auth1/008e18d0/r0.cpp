// NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams() -- aim the storm's beams
// Leaf: ?UpdateLightningBeams@CModeThunderLightningStorm@NCreatureMode@@AAEXXZ
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_UNINIT_FASTCALL
#define FABLE_RETAIL_CCRC_PASS22_BODY
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_IMPLICIT_DTOR
#include <string.h>
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCRC.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CRightHandedSet.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/NCreatureMode_CModeThunderLightningStorm.h"

void NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams()
{
    long num_beams = LightningBeamParticleEmitters.size();
    float angle_between_beams = 1.0f / num_beams;
    long storm_rotation_int = (long)StormRotation;
    float storm_rotation = StormRotation - storm_rotation_int;
    float beam_length = StormRadius;
    C3DVector weapon_pos = ParentThing.GetPos();
    CRightHandedSet rhs;
    ParentThing.GetTCGraphicAppearance()->GetDummyObject("WEAPON_FOCUS_01", 0, 0, weapon_pos, rhs, 1.0f);
    C3DVector start_pos = weapon_pos;
    long n;
    for (n = 0; n < num_beams; ++n) {
        if (IS_THING_ALIVE(LightningBeamParticleEmitters[n])) {
            C3DVector beam_direction(0.0f, 1.0f, 0.0f);
            beam_direction.RotateXY(n * angle_between_beams + storm_rotation);
            C3DVector end_pos = beam_direction * beam_length + ParentThing.GetPos();
            end_pos.Z = ParentThing.GetPos().Z;
            CTCDParticleEmitter* ptc_particle_emitter = 0;
            LightningBeamParticleEmitters[n]->GetTC(&ptc_particle_emitter);
            std::pair<unsigned long, C3DVector> vtStartPos(CCRC::Calc(0, "LIGHTNING_START", strlen("LIGHTNING_START")), start_pos);
            std::pair<unsigned long, C3DVector> vtEndPos(CCRC::Calc(0, "LIGHTNING_END", strlen("LIGHTNING_END")), end_pos);
            NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> upParams;
            upParams.VectorParams.Insert(vtStartPos);
            upParams.VectorParams.Insert(vtEndPos);
            upParams.Update();
            ptc_particle_emitter->SetEmitterUpdateParameters(upParams);
        }
    }
    long num_beams_ends = LightningBeamEndParticleEmitters.size();
    for (n = 0; n < num_beams_ends; ++n) {
        if (IS_THING_ALIVE(LightningBeamEndParticleEmitters[n])) {
            C3DVector beam_direction(0.0f, 1.0f, 0.0f);
            beam_direction.RotateXY(n * angle_between_beams + storm_rotation);
            C3DVector end_pos = beam_direction * beam_length + ParentThing.GetPos();
            end_pos.Z = ParentThing.GetPos().Z;
            CTCDParticleEmitter* ptc_particle_emitter = 0;
            LightningBeamEndParticleEmitters[n]->GetTC(&ptc_particle_emitter);
            ptc_particle_emitter->SetEmitterPosition(end_pos);
        }
    }
    if (IS_THING_ALIVE(PSkyBeam)) {
        CTCDParticleEmitter* ptc_particle_emitter = 0;
        PSkyBeam->GetTC(&ptc_particle_emitter);
        C3DVector end_pos = start_pos;
        end_pos.Z += 15.0f;
        std::pair<unsigned long, C3DVector> vtStartPos(CCRC::Calc(0, "LIGHTNING_START", strlen("LIGHTNING_START")), start_pos);
        std::pair<unsigned long, C3DVector> vtEndPos(CCRC::Calc(0, "LIGHTNING_END", strlen("LIGHTNING_END")), end_pos);
        NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> upParams;
        upParams.VectorParams.Insert(vtStartPos);
        upParams.VectorParams.Insert(vtEndPos);
        upParams.Update();
        ptc_particle_emitter->SetEmitterUpdateParameters(upParams);
    }
}
