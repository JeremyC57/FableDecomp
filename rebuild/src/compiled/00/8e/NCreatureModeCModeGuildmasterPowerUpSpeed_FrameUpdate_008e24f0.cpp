// NCreatureMode::CModeGuildmasterPowerUpSpeed::FrameUpdate() -- base frame update and timer tick; aim each guard's lightning beam from the midpoint of the guildmaster's hands to 1.75 above the guard (killing the beam once its guard is gone); when the timer has run out or the boss has no living helpers left, start the "speed-up followers release" action
// Leaf: ?FrameUpdate@CModeGuildmasterPowerUpSpeed@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E24F0, 1023 bytes, thiscall, plain `ret`; vtable cell 0x01268630 (slot 3).
// Identity: Ego_r.pdb ?FrameUpdate@CModeGuildmasterPowerUpSpeed@NCreatureMode@@UAEXXZ; the vtable's other slots include the landed
//   OnDeactivate 0x008D7B20; the action's vtable 0x01286C4C is CCombatAction_Guildmaster_SpeedUpFollowersRelease. Same body as the
//   Shield mode's 0x008E20F0 except that the boss is looked up right after the timer tick.
// Callees: CModeBase::FrameUpdate 0x0073AC50, CTCGraphicAppearance::GetHelperPoint 0x004C0560, CBaseIntelligentPointer::GetPItem
//   0x00A01B50, CThing::GetPos 0x004C73D0 / Kill 0x004C9B80, CVectorMap::LowerBound 0x0040F020 (inline GetTC), CCRC::Calc 0x004014A0,
//   CBaseClass() 0x0099A2D0, vector<pair<unsigned long, C3DVector> >::_M_insert_overflow 0x0076ADA0, std::sort 0x0076C000 / 0x0076C1A0,
//   CTCDParticleEmitter::SetEmitterUpdateParameters 0x006E11A0, ~CParticleUpdateParameters 0x006E2500, CTCBoss::CountLivingHelpers
//   0x007AEEC0, CCombatActionBase::GetSoundPair 0x008561A0 / CCombatActionBase(CThingCreatureBase&, long, const CSoundPair&) 0x00858030 /
//   ~CCombatActionBase 0x0062E900, CThingCreatureBase::SetCurrentAction 0x006644F0, CCharString 0x0099EBF0 / 0x0099EAE0.
// Data: "weapon_pos_left_hand" 0x01276500, "weapon_pos_right_hand" 0x01276518, "LIGHTNING_START" 0x0126C810, "LIGHTNING_END"
//   0x0126C800, "SWORD_SWING" 0x0127AC80, 0.5f 0x0122F59C, 1.75f 0x012657FC.
// Headers (overlay, FABLE_HDR_OVERLAY=$A/hdr): new engine/NCreatureMode_CModeGuildmasterPowerUpShield.h (both modes),
//   engine/NParticleEngine_CParticleUpdateParameters.h, engine/CCombatAction_Guildmaster_FollowersRelease.h; opt-in FABLE_RETAIL_CTCBOSS_LIVINGHELPERS.
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
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCBOSS_LIVINGHELPERS
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#include <string.h>
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCRC.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/ThingAlive.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCombatAction_Guildmaster_FollowersRelease.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCBoss.h"
#include "engine/CThingAICreature.h"
#include "engine/NCreatureMode_CModeGuildmasterPowerUpShield.h"

void NCreatureMode::CModeGuildmasterPowerUpSpeed::FrameUpdate()
{
    CModeBase::FrameUpdate();
    --Timer;
    CTCBoss* boss;
    ParentThing.GetTC(&boss);
    // the beams start between the guildmaster's hands
    C3DVector leftHand;
    ParentThing.GetTCGraphicAppearance()->GetHelperPoint("weapon_pos_left_hand", 0, 0, leftHand, 1.0f);
    C3DVector rightHand;
    ParentThing.GetTCGraphicAppearance()->GetHelperPoint("weapon_pos_right_hand", 0, 0, rightHand, 1.0f);
    C3DVector centre = (leftHand + rightHand) * 0.5f;
    long count = Guards.size();
    for (long i = 0; i < count; ++i) {
        if (IS_THING_ALIVE(LightningBeamParticleEmitters[i])) {
            if (IS_THING_ALIVE(Guards[i])) {
                // the beam ends 1.75 above its guard
                C3DVector end = Guards[i]->GetPos();
                end.Z = Guards[i]->GetPos().Z + 1.75f;
                CTCDParticleEmitter* emitter = 0;
                LightningBeamParticleEmitters[i]->GetTC(&emitter);
                std::pair<unsigned long, C3DVector> startParam(CCRC::Calc(0, "LIGHTNING_START", strlen("LIGHTNING_START")), centre);
                std::pair<unsigned long, C3DVector> endParam(CCRC::Calc(0, "LIGHTNING_END", strlen("LIGHTNING_END")), end);
                NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> parameters;
                parameters.VectorParams.Insert(startParam);
                parameters.VectorParams.Insert(endParam);
                parameters.Update();
                emitter->SetEmitterUpdateParameters(parameters);
            } else {
                LightningBeamParticleEmitters[i]->Kill(true);
            }
        }
    }
    if (Timer <= 0 || boss->CountLivingHelpers() == 0) {
        CThingCreatureBase& creature = static_cast<CThingCreatureBase&>(ParentThing);
        creature.SetCurrentAction(CCombatAction_Guildmaster_SpeedUpFollowersRelease(creature));
    }
}