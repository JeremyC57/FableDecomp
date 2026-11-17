// CAIStateGroup_ApprenticeHero::LookAtLeaderBoard() -- walk to the leaderboard marker and study the leaderboard
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Leaf: ?LookAtLeaderBoard@CAIStateGroup_ApprenticeHero@@IAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092AE70, length 354 bytes.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("Leaderboard" @ 0x01291BB0, "LeaderboardMarker" @ 0x01291B9C,
//          "ST_TRAINEE_LOOK_AT_MAP_TABLE" @ 0x01291B7C), the marker finder @ 0x00929FC0, ~CCharString @ 0x0099EAE0,
//          CTCPhysicsBase::GetBoundingInfo @ 0x006AF640, AIAction_MoveToPosProximity @ 0x0097E890, CIntelligentPointer<const
//          CThing>(CThing*) @ 0x0056C860 (built in the argument slot), AIAction_LookAtThing @ 0x0097D6F0,
//          CCreatureAction_PlayCombatAnimation @ 0x008B7FF0, AIAction_PerformActionAndWait @ 0x0097DB70, ~CCombatActionBase @ 0x0062E900.
// Globals: the leaderboard-in-use flag @ 0x013BB6B0 and the approach distance @ 0x013BB6AC (set to 0.5f), the look turn
//          speed @ 0x0138CBA8.
// Shape: both finds use block-scoped names in the same frame slot; a missing board or marker counts as success; the
//          approach distance global is written just before the walk (VC schedules the store among the pushes); the look
//          target is the converting intelligent pointer built in place; the animation string is block-scoped (it shares
//          the find names' slot) and its result a named bool returned as `failed ? false : true` (no `xor eax,eax`).
// Layout: engine/CAIStateGroup_ApprenticeHero.h behind engine/CAIStateGroupBase.h; the marker's physics at +0x60.
// NF51 (round 7): the AI state-group base, the apprentice class, the string, the intelligent pointer, the things, the physics
// component and the combat-animation action come from the shared headers, as in the same-TU sibling GoToLibrary 0x0092ACE0 (were
// local models). The callee 0x0097E890 is the protected
// ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z
// (Ego_r.pdb / FableWin.pdb; _MOVETOPOSPROXIMITY) with the Ego_r enums of engine/fable_ai_action_enums.h; AIAction_LookAtThing 0x0097D6F0
// (_AIACTIONS) takes the shared CIntelligentPointer<const CThing>; AIAction_PerformActionAndWait 0x0097DB70 (_PERFORMACTION) takes the
// action by const reference; GetBoundingInfo 0x006AF640 is the header's const (C3DVector&, float&); the finder 0x00929FC0 is
// ?GetObjectWithScriptName@CAIStateGroup_ApprenticeHero@@IAEPAVCThingGameObject@@ABVCCharString@@@Z (_SCRIPTNAME). The function itself
// is declared by the overlay opt-in FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO_LOOKATLEADERBOARD (protected, Ego_r.pdb / FableWin.pdb).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO
#define FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO_SCRIPTNAME
#define FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO_LOOKATLEADERBOARD
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_ApprenticeHero.h"

extern float g_LookAtTurnSpeed;            // 0x0138CBA8 (the look turn speed shared with GoToLibrary)
extern bool g_LeaderboardInUse;                 // 0x013BB6B0
// The approach distance is TU-local: the only reference to 0x013BB6AC in the whole executable is this function's
// store, so it is a file-scope static of this TU (as an extern, VC7.1 schedules the store ahead of the by-value
// vector copy; retail stores it after `push 1 ; mov ecx,esi`).
static float g_LeaderboardApproachDistance;     // 0x013BB6AC

bool CAIStateGroup_ApprenticeHero::LookAtLeaderBoard()
{
    if (g_LeaderboardInUse)
        return true;
    CThingGameObject* board;
    {
        CCharString name("Leaderboard");
        board = GetObjectWithScriptName(name);
    }
    CThingGameObject* marker;
    {
        CCharString name("LeaderboardMarker");
        marker = GetObjectWithScriptName(name);
    }
    if (!marker || !board)
        return true;
    C3DVector centre;
    float radius;
    marker->GetPhysicsTC()->GetBoundingInfo(centre, radius);
    bool reached;
    if (AIAction_MoveToPosProximity(ACTION_REPEAT_UNTIL_FINISHED, centre, g_LeaderboardApproachDistance = 0.5f, reached, AI_MOVEMENT_WALKING, true, false, true, 0, true, 0.0f) != 2)
        return false;
    if (AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, CIntelligentPointer<const CThing>(board), 0xb, g_LookAtTurnSpeed) != 2)
        return false;
    bool failed;
    {
        CCharString animation("ST_TRAINEE_LOOK_AT_MAP_TABLE");
        failed = AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), animation, true), false) != 2;
    }
    return failed ? false : true;
}