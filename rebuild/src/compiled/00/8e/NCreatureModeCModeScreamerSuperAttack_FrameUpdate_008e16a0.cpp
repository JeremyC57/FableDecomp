// NCreatureMode::CModeScreamerSuperAttack::FrameUpdate() -- the screamer's lightning super attack: run the base mode update; while
//   the lightning-beam emitter lives, aim it from the screamer's focus_eye_01 helper point to 1.75 above the live target (the
//   LIGHTNING_START / LIGHTNING_END vector parameters), or kill the beam once the target is gone
// Leaf: ?FrameUpdate@CModeScreamerSuperAttack@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E16A0, 552 bytes, thiscall, plain `ret`; slot 3 of the mode vtable 0x01278338.
// Identity: Ego_r.pdb ?FrameUpdate@CModeScreamerSuperAttack@NCreatureMode@@UAEXXZ (Ego_r 0x00823880); the catalog name.
// Callees: CModeBase::FrameUpdate 0x0073AC50, CBaseIntelligentPointer::GetPItem 0x00A01B50, CCharString(const char*, long) 0x0099EBF0 /
//   ~ 0x0099EAE0, NGraphicAppearance::CTCGraphicAppearance::GetHelperPoint 0x004C0560, CThing::GetPos 0x004C73D0, CVectorMap LowerBound
//   0x0040F020 (GetTC<CTCDParticleEmitter>, TC 75), CCRC::Calc 0x004014A0 (__fastcall), CBaseClass ctor 0x0099A2D0 (the parameters'
//   base), vector<pair<unsigned long,C3DVector> >::_M_insert_overflow 0x0076ADA0, CParticleUpdateParameters::Update 0x0076C750,
//   CTCDParticleEmitter::SetEmitterUpdateParameters 0x006E11A0, ~CParticleUpdateParameters 0x006E2500, CThing::Kill 0x004C9B80.
// Shape: the landed sibling CModeGuildmasterPowerUpShield::FrameUpdate 0x008E20F0's beam block (its define set), with one GetPos read
//   for the end point (retail reads the target position once: `fld [eax+8]` then the X / Y copies, 0x008E173F).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_INSERT_FORCEINLINE
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#include <string.h>
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCRC.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/ThingAlive.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/NCreatureMode_CModeScreamerSuperAttack.h"

void NCreatureMode::CModeScreamerSuperAttack::FrameUpdate()
{
    CModeBase::FrameUpdate();
    if (IS_THING_ALIVE(PLightningBeamParticleEmitter)) {
        if (IS_THING_ALIVE(PTarget)) {
            C3DVector start;
            ParentThing.GetTCGraphicAppearance()->GetHelperPoint("focus_eye_01", 0, 0, start, 1.0f);
            const C3DVector& target_pos = PTarget->GetPos();
            C3DVector end(target_pos.X, target_pos.Y, target_pos.Z + 1.75f);
            CTCDParticleEmitter* emitter = 0;
            PLightningBeamParticleEmitter->GetTC(&emitter);
            std::pair<unsigned long, C3DVector> startParam(CCRC::Calc(0, "LIGHTNING_START", strlen("LIGHTNING_START")), start);
            std::pair<unsigned long, C3DVector> endParam(CCRC::Calc(0, "LIGHTNING_END", strlen("LIGHTNING_END")), end);
            NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> parameters;
            parameters.VectorParams.Insert(startParam);
            parameters.VectorParams.Insert(endParam);
            parameters.Update();
            emitter->SetEmitterUpdateParameters(parameters);
        } else {
            PLightningBeamParticleEmitter->Kill(true);
        }
    }
}