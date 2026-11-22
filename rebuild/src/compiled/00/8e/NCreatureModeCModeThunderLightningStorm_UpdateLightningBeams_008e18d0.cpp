// NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams() -- aim the storm's beams: from the caster's WEAPON_FOCUS_01 dummy,
//   main beam n points along (0,1,0) turned n / num_beams plus the fraction of StormRotation, StormRadius long at the caster's height
//   (LIGHTNING_START / LIGHTNING_END vector parameters); each live end beam is moved to its end point; a live sky beam rises 15 above
//   the start. Dead or missing emitters are skipped.
// Leaf: ?UpdateLightningBeams@CModeThunderLightningStorm@NCreatureMode@@AAEXXZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E18D0, 2073 bytes, thiscall, plain `ret`. Grade RELOCATION_MATCH at /Gr, default inline
//   threshold, with FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_IMPLICIT_DTOR (AUTH14 + coordinator, AUTH15 2026-11-22).
// Identity: Ego_r.pdb 0x00823B70 (release\creature_modes.obj #186, 2310 B) / FableWin.pdb 0x0285AA60 (debug\creature_modes.obj #230,
//   3407 B, = Ego_d's record) ?UpdateLightningBeams@CModeThunderLightningStorm@NCreatureMode@@AAEXXZ (private).
// Flags evidence: creature_modes.obj was compiled with -Gr (Ego_d.pdb carved S_ENVBLOCK and FableWin.pdb module 212 S_ENVBLOCK, the
//   odall10 fablelib preset); in this TU the STL helpers _M_insert_overflow 0x0076ADA0 / __uninitialized_copy 0x00768FC0 /
//   __uninitialized_fill_n 0x00769000 compare SAME only under /Gr (fastcall, ecx/edx).
// Callees: CThing::GetPos 0x004C73D0 (x5), CTCGraphicAppearance::GetDummyObject 0x004C08C0, CCharString ctor / dtor 0x0099EBF0 /
//   0x0099EAE0, __ftol2 0x00BFEA70, CBaseIntelligentPointer::GetPItem 0x00A01B50, the component-map LowerBound 0x0040F020 (loops 1, 2;
//   the sky's is expanded), C3DVector::RotateXY 0x00A14040, CBaseClass ctor / dtor 0x0099A2D0 / 0x0099A2E0, vector<pair<ulong,
//   C3DVector> >::_M_insert_overflow 0x0076ADA0 (loop 1; the sky's two are expanded, malloc 0x00BFEA0E / free 0x00BFEA14), std::sort
//   0x0076C000 / 0x0076C1A0 (Update), CTCDParticleEmitter::SetEmitterUpdateParameters 0x006E11A0 / SetEmitterPosition 0x006E0500,
//   ~CParticleUpdateParameters<ulong,ulong> 0x006E2500 (loop 1 at 0x008E1C3A; expanded in the sky block). CCRC::Calc is expanded at all four keys.
// Locals: Ego_d.pdb's names, block by block (function scope 12; loop-1 block 6; loop-2 block 3; sky block 5). lifetime_in_seconds /
//   lifetime_of_main_beams are computed and never read (Ego_d lines 3283 / 3285); retail drops them.
// Inferred: `storm_rotation -= storm_rotation_int` (`storm_rotation = storm_rotation - storm_rotation_int` gives the same bytes); the VC6
//   for-scope idiom `if (false) {} else for` on both loops (Ego_d 3302 / 3332 = 39 B, as the TU-mate 0x008DD7E0).
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
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_VECTORKEY_PASS26
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
#include "engine/CSpecialAbilitiesThunderLightningStormDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/NCreatureMode_CModeThunderLightningStorm.h"

void NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams()
{
    long num_beams = LightningBeamParticleEmitters.size();
    float angle_between_beams = 1.0f / num_beams;
    float lifetime_in_seconds = (float)Timer / CMainGameComponent::GetConstantFPS();
    float lifetime_of_main_beams = lifetime_in_seconds - PLightningStormDef->MainBeamsStartTime;
    float storm_rotation = StormRotation;
    long storm_rotation_int = (long)storm_rotation;
    storm_rotation -= storm_rotation_int;
    float beam_length = StormRadius;
    CRightHandedSet rhs;
    C3DVector weapon_pos = GetParentThing().GetPos();
    GetParentThing().GetTCGraphicAppearance()->GetDummyObject("WEAPON_FOCUS_01", 0, 0, weapon_pos, rhs, 1.0f);
    C3DVector start_pos = weapon_pos;
    long n;
    if (false) {} else for (n = 0; n < num_beams; ++n) {
        if (IS_THING_ALIVE(LightningBeamParticleEmitters[n])) {
            C3DVector beam_direction(0.0f, 1.0f, 0.0f);
            beam_direction.RotateXY(n * angle_between_beams + storm_rotation);
            C3DVector end_pos = beam_direction * beam_length + GetParentThing().GetPos();
            end_pos.Z = GetParentThing().GetPos().Z;
            CTCDParticleEmitter* ptc_particle_emitter = 0;
            LightningBeamParticleEmitters[n]->GetTC(&ptc_particle_emitter);
            std::pair<unsigned long, C3DVector> vtStartPos(NParticleEngine::MAKE_VECTOR_PARAM_FROM_BASE(0, "LIGHTNING_START"), start_pos);
            std::pair<unsigned long, C3DVector> vtEndPos(NParticleEngine::MAKE_VECTOR_PARAM_FROM_BASE(0, "LIGHTNING_END"), end_pos);
            NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> upParams;
            upParams.VectorParams.Insert(vtStartPos);
            upParams.VectorParams.Insert(vtEndPos);
            upParams.Update();
            ptc_particle_emitter->SetEmitterUpdateParameters(upParams);
        }
    }
    long num_beams_ends = LightningBeamEndParticleEmitters.size();
    if (false) {} else for (n = 0; n < num_beams_ends; ++n) {
        if (IS_THING_ALIVE(LightningBeamEndParticleEmitters[n])) {
            C3DVector beam_direction(0.0f, 1.0f, 0.0f);
            beam_direction.RotateXY(n * angle_between_beams + storm_rotation);
            C3DVector end_pos = beam_direction * beam_length + GetParentThing().GetPos();
            end_pos.Z = GetParentThing().GetPos().Z;
            CTCDParticleEmitter* ptc_particle_emitter = 0;
            LightningBeamEndParticleEmitters[n]->GetTC(&ptc_particle_emitter);
            ptc_particle_emitter->SetEmitterPosition(end_pos);
        }
    }
    if (IS_THING_ALIVE(PSkyBeam)) {
        CTCDParticleEmitter* ptc_particle_emitter = 0;
        PSkyBeam->GetTC(&ptc_particle_emitter);
        C3DVector end_pos = start_pos + C3DVector(0.0f, 0.0f, 15.0f);
        std::pair<unsigned long, C3DVector> vtStartPos(NParticleEngine::MAKE_VECTOR_PARAM_FROM_BASE(0, "LIGHTNING_START"), start_pos);
        std::pair<unsigned long, C3DVector> vtEndPos(NParticleEngine::MAKE_VECTOR_PARAM_FROM_BASE(0, "LIGHTNING_END"), end_pos);
        NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> upParams;
        upParams.VectorParams.Insert(vtStartPos);
        upParams.VectorParams.Insert(vtEndPos);
        upParams.Update();
        ptc_particle_emitter->SetEmitterUpdateParameters(upParams);
    }
}