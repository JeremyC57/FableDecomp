// 007ef710 CTCDecapitation::Decapitate
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_GROUPED
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ATTACKEDBY
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_CLASSTYPE

#define FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTHING_ADDCOMPONENT
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_PERSONALITY
#define FABLE_RETAIL_CENGINETEXTUREMORPHSET
#define FABLE_RETAIL_CTCAPPEARANCEMORPHBASE
#define FABLE_RETAIL_CTCAPPEARANCEMORPHBASE_MESHMORPHSET
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_NETWORK
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD
#define FABLE_RETAIL_CTCSOLIDTOTHING
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FORCERESEND
#define FABLE_RETAIL_DECAPITATE_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_APPLYACCELERATION
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CDECAPITATIONDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE_GETPARENTTHING
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
// Leaf: ?Decapitate@CTCDecapitation@@QAEXXZ
// Flags: /O2 /Gr
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ROTATE_ANGLES
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"

#include "engine/ETCInterfaceType.h"
#include "engine/CTCDecapitation.h"
#include "engine/CDecapitationDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingObject.h"
#include "engine/ThingAlive.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCDecapitation.h"
#include "engine/CDecapitationDef.h"
#include "engine/CTCRotate.h"
#include "engine/CTCSolidToThing.h"
#include "engine/CTCHeroOnlineScoreboard.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCRandomAppearanceMorph.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
namespace {
extern CCharString DECAPITATION_DUMMY_OBJECT_NAME;
extern C3DVector MISSING_DUMMY_OBJECT_BODGE_VECTOR;
const float INITIAL_HEAD_ACCELERATION_MAGNITUDE=0.4f;
}
void CTCDecapitation::Decapitate()
{
    if (Decapitated)
        return;
    if (GetParentThing().GetTypeFlags() & 8) {
        const CThingCreatureBase& parent_creature=static_cast<const CThingCreatureBase&>(GetParentThing());
        CThing* plast_attacker=parent_creature.GetPLastAttackedByCreature();
        CTCHeroStats* ptc_hero_stats;
        if (IS_THING_ALIVE(plast_attacker) && plast_attacker->GetTC(&ptc_hero_stats)) {
            ptc_hero_stats->SetNumberOfDecapitations(ptc_hero_stats->GetNumberOfDecapitations()+1);
            CTCHeroOnlineScoreboard* ptc_hero_scoreboard;
            plast_attacker->GetTC(&ptc_hero_scoreboard);
            ptc_hero_scoreboard->UpdateScoreForActivity(CTCHeroOnlineScoreboard::OSA_DECAPITATIONS,(float)ptc_hero_stats->GetNumberOfDecapitations(),false);
        }
    }
    if(!GetParentThing().HasTC<CTCRandomAppearanceMorph>())return;
    Decapitated=true;
    CTCRandomAppearanceMorph* ptc_random_appearance_morph;
    GetParentThing().GetTC(&ptc_random_appearance_morph);
    ptc_random_appearance_morph->UpdateDecapitationStatus();
    C3DVector headPos;
    CRightHandedSet headSet;
    bool found = GetParentThing().GetTCGraphicAppearance()->GetDummyObject(DECAPITATION_DUMMY_OBJECT_NAME, 0, 0, headPos, headSet, 1.0f);
    if (!found) {
        headPos=GetParentThing().GetPos()+MISSING_DUMMY_OBJECT_BODGE_VECTOR;
        headSet = GetParentThing().GetPhysicsTC()->GetRHSet();
    }
    if (PDef->Head) {
        CThingObject* head = CThingObject::Create(PDef->Head, headPos, GetParentThing().GetPlayerNumber(), &headSet, 0, "");
        if (IS_THING_ALIVE(head)) {
            CTCSolidToThing* solid = (CTCSolidToThing*)head->AddTC("CTCSolidToThing", true, 0);
            solid->AddThingNotSolidTo(GetPlayerManager().GetMainPlayer().GetPControlledCreature());
            C3DVector initial_head_acceleration=GetParentThing().GetPhysicsTC()->GetFacingVector()+C3DVector(0.0f,0.0f,0.3f);
            initial_head_acceleration.Normalise();
            head->GetPhysicsTC()->ApplyAcceleration(initial_head_acceleration*INITIAL_HEAD_ACCELERATION_MAGNITUDE);
            CTCRotate* rotate = (CTCRotate*)head->AddTC("CTCRotate", false, 0);
            if (rotate) {
                C3DVector axis(0.0f, 0.0f, 1.0f);
                axis.Rotate(0.0f, 0.0f);
                rotate->SetRotationInfo(axis, 0.05f, 2.2f);
                rotate->SetRotating(true);
            }
            head->GetTCGraphicAppearance()->SetForceResendEveryRenderFrame(true);
        }
    }
    if (PDef->BodyParticleEffect > 0) {
        CThing* blood = CTCDParticleEmitter::Create(PDef->BodyParticleEffect, headPos, false);
        if (IS_THING_ALIVE(blood)) {
            CTCDParticleEmitter* emitter;
            blood->GetTCDriver(&emitter);
            if (found)
                emitter->AttachToThing(GetParentThing(), 0xa, DECAPITATION_DUMMY_OBJECT_NAME, 0, 0.0f);
            else
                emitter->AttachToThing(GetParentThing(), 0xa8, "", 0, MISSING_DUMMY_OBJECT_BODGE_VECTOR.Z);
        }
    }
}