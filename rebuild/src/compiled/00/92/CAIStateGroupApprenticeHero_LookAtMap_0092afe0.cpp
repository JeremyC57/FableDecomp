// CAIStateGroup_ApprenticeHero::LookAtMap() -- walk to the guild map, turn to it and play the trainee look-at-map animation; true when there is no map
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): the remembered map radius is a function-local static (its store
//   schedules after the radius argument push, an extern global's before) and the tail is if (... != 2) return false;
//   return true; (setne bl / sete al)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092AFE0, 304 bytes (`ret`). Ego_r.pdb: ?LookAtMap@CAIStateGroup_ApprenticeHero@@IAE_NXZ.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("MainGuildMap" 0x01291BBC,
// "ST_TRAINEE_LOOK_AT_MAP_TABLE" 0x01291B7C), CAIStateGroup_ApprenticeHero::GetObjectWithScriptName(const CCharString&)
// 0x00929FC0 (manifest "CAIStateGroupBase_SelectRandomElement" is propagated), CTCPhysicsBase::GetBoundingInfo
// (C3DVector*, float*) 0x006AF640, AIAction_MoveToPosProximity 0x0097E890, AIAction_LookAtThing 0x0097D6F0 (by-value
// CIntelligentPointer<const CThing> built in its argument slot: ctor 0x00A01B10, vtable 0x01237F1C, SetPItem 0x00A01B90),
// CCreatureAction_PlayCombatAnimation(CThingCreatureBase*, const CCharString&, bool) 0x008B7FF0 / ~CCombatActionBase
// 0x0062E900, AIAction_PerformActionAndWait 0x0097DB70; globals: the last map radius 0x013BB6B4, the look-at turn
// speed 0x0138CBA8 (0.01f, read as a variable).
// Shape: implicit const char* -> CCharString temporaries (re-addressed with `lea`), `bool reached;` uninitialised in
// the high byte of a frame word, the move with the as-landed AIAction_MoveToPosProximity arguments, the look-at,
// then the animation action as an address-of-temporary (as the landed 0x009574D0) and `== 2` returned after the dtors.
// Leaf: ?LookAtMap@CAIStateGroup_ApprenticeHero@@IAE_NXZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Name fix (agentNF23): SetPItem takes the PDB CBaseObject* (Ego_r.pdb / FableWin.pdb ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z, 0x00A01B90); the argument is cast to CBaseObject* as CIntelligentPointer<T>::SetPointer does.
// NF51 (round 7): the AI state-group base, the apprentice class, the string, the intelligent pointer, the things, the physics
// component and the combat-animation action come from the shared headers, as in the same-TU sibling GoToLibrary 0x0092ACE0 (were
// local models). The callee 0x0097E890 is the protected
// ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z
// (Ego_r.pdb / FableWin.pdb; _MOVETOPOSPROXIMITY) with the Ego_r enums of engine/fable_ai_action_enums.h; AIAction_LookAtThing 0x0097D6F0
// (_AIACTIONS) takes the shared CIntelligentPointer<const CThing>; AIAction_PerformActionAndWait 0x0097DB70 (_PERFORMACTION) takes the
// action by const reference; GetBoundingInfo 0x006AF640 is the header's const (C3DVector&, float&); the finder 0x00929FC0 is
// ?GetObjectWithScriptName@CAIStateGroup_ApprenticeHero@@IAEPAVCThingGameObject@@ABVCCharString@@@Z (_SCRIPTNAME). The function itself
// is declared by the overlay opt-in FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO_LOOKATMAP (protected, Ego_r.pdb / FableWin.pdb).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR
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
#define FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO_LOOKATMAP
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

extern float g_LookAtTurnSpeed;                    // 0x0138CBA8 (0.01f)

bool CAIStateGroup_ApprenticeHero::LookAtMap()
{
    CThingGameObject* map = GetObjectWithScriptName("MainGuildMap");
    if (!map)
        return true;
    C3DVector centre;
    float radius;
    CTCPhysicsBase* physics = map->GetPhysicsTC();
    physics->GetBoundingInfo(centre, radius);
    bool reached;
    static float lastMapRadius;                    // 0x013BB6B4 (.bss; descriptive name, no PDB symbol found)
    lastMapRadius = radius;
    if (AIAction_MoveToPosProximity(ACTION_REPEAT_UNTIL_FINISHED, centre, radius, reached, AI_MOVEMENT_WALKING, true, false, true, 0, true, 0.0f) != 2)
        return false;
    if (AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, CIntelligentPointer<const CThing>(map), 0xb, g_LookAtTurnSpeed) != 2)
        return false;
    if (AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), "ST_TRAINEE_LOOK_AT_MAP_TABLE", true), false) != 2)
        return false;
    return true;
}