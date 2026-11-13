// CThingCreatureBase::UpdateAnimation(float time) -- per-frame creature animation step: builds the frame's animation update (time and xy distance moved, scaled by the speed multiplier); after a long gap it resets to the DEFAULT pose, otherwise when the main animation is free it lets a hovering creature fly / glide or the current mode pick the main animation (unless the current action's own animation is no combination animation, whose leftover combination set fades), then refreshes the default face, the look, expression and talk layers, the action's animation, and advances the animator
// Leaf: ?UpdateAnimation@CThingCreatureBase@@IAEXM@Z
// Retail: Fable.exe 0x00665860, 1405 bytes (`ret 4` at 0x00665DDA; the scanned extent; then 3 int3 and the unlisted 8-byte forwarder
//   0x00665DE0 UpdateAnimationBulletTime: `push 0 ; call 0x00665860 ; ret`, so the row length 1405 is right). Ego_r.pdb / FableWin.pdb
//   ?UpdateAnimation@CThingCreatureBase@@IAEXM@Z (protected; callers 0x00665DE2, 0x006ADA7F, 0x0082FDF0, 0x0083485F).
// Callees: CTCAnimationComplex::GetAnimation 0x0070B460; CThing::PeekComponent 0x004C7A70 (its result only names the static
//   CMainGameComponent::ConstantFPS 0x01375550); GetAnimationSpeedMultiplier 0x006655A0; CTCCreatureModeManager::GetCurrentMode 0x0073A6E0
//   (CModeBase slot 1 GetType, slot 2 AnimUpdate); CAnimateThing ClearAllAnims 0x0070EC30, SetMainAnim 0x0070D580, IsMainAnimFinished
//   0x0070C0E0, GetFirstCombinationAnimFromType 0x0070C4E0, AddCombinationAnim 0x0070D780, RemoveCombinationAnimsOfType 0x0070D8C0,
//   Update 0x0070E8B0; CAnimSet::IsFinished 0x00668A80; GetAnimFromName 0x00662A00; IsAnimCombinationAnim 0x00662820;
//   CAnimationEntry GetAnimateThingFlags 0x005DC390, GetDelayTime 0x005DC1F0, GetTimeToBlendIn 0x005DC230; CAnimInit ctor 0x005B33F4 /
//   Init 0x0070C050; CTCCreatureNavigation::GetNavigatorType 0x006CB6A0; CTCLook::IsInsideAnimationRange 0x00751BC0;
//   CTCCreatureExpression::Update 0x00729200; CTCTalk::Update 0x006E3B90; CCreatureActionBase::UpdateActionAnimation 0x00694890
//   (GetAnimationIndex = vtable slot 7); AnimExists = vtable slot 73.
// Shape: the xy speed is C3DVector::GetSquaredMagnitude2D() (the FableWin inline accessor: Y and X stay on the x87 stack and are
//   squared there, 0x006658D6; a C2DVector temporary is copied through integer registers and squared after the calls); the frame
//   time is `1.0f / PeekComponent().GetConstantFPS()` (the static read through the object expression keeps retail's PeekComponent
//   call, 0x006658F6); the update is built before `IdleCounter = 0` (its zero PercentageComplete store comes first, 0x0066591A); the
//   current mode's GetType (slot 1) is called for nothing, as retail does (0x00665949); the main-anim switch is an if / else with
//   a flag cleared by the action / main-type tests; the two expression / talk gates are `!(rate * N > time)` (retail branches with
//   `je` after `test ah,41h`: NaN takes the remove path; `<=` gives `jp`); the look lookup is the non-const GetTC (LowerBound
//   0x0040F020). The DEFAULT reset builds a named CAnimInit (its constructor expanded: Frame zeroed with integer stores, then Init
//   0x0070C050 and `init.Speed = 0.0f`); the fly / glide / face sites pass CAnimInit temporaries, whose constructor VC7.1 calls out
//   of line (0x005B33F4) as retail does.
// Globals: 0x013B8774 and 0x0129CB44 have no PDB data name (no data-symbol dump); the established descriptive spellings are kept.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGCREATUREBASE_UPDATEANIMATION
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_INTERRUPTION
#define FABLE_RETAIL_CCREATUREACTIONBASE_UPDATEANIM
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGATORTYPE
#define FABLE_RETAIL_CTCTALK_UPDATE
#define FABLE_RETAIL_LOOK_HEAD_ANIM
#define FABLE_RETAIL_CANIMSET_TYPE
#define FABLE_RETAIL_CANIMATETHING_MAINANIM
#define FABLE_RETAIL_CANIMATETHING_CLEARALLANIMS
#define FABLE_RETAIL_CANIMATETHING_UPDATEANIMATION
#define FABLE_RETAIL_CANIMATIONENTRY_ACCESSORS
#define FABLE_RETAIL_CANIMATIONENTRY_BLENDTIMES
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_CLOOKDEF
#include <math.h>
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CAnimInit.h"
#include "engine/CAnimSet.h"
#include "engine/CAnimateThing.h"
#include "engine/CAnimateThingUpdate.h"
#include "engine/CAnimationEntry.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCLook.h"
#include "engine/CTCCreatureExpression.h"
#include "engine/CTCTalk.h"
#include "fable_game.h"

extern float g_primitiveDrawDistanceScale;                               // 0x013B8774 (established descriptive spelling)
extern unsigned long FlyingNavigatorType;                                // 0x0129CB44 (established descriptive spelling)

void CThingCreatureBase::UpdateAnimation(float time)
{
    CTCAnimationComplex* animation_tc;
    if (!GetTC(TCI_ANIMATION, &animation_tc))
        return;
    CAnimateThing& animation = animation_tc->GetAnimation();
    CTCCreatureModeManager* modes = 0;
    GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
    float speed_squared = MovementVector.GetSquaredMagnitude2D();
    float frame_time = 1.0f / PeekComponent().GetConstantFPS();
    float multiplier = GetAnimationSpeedMultiplier();
    float distance = sqrtf(speed_squared) * multiplier;
    CAnimateThingUpdate update(multiplier * frame_time, distance);
    IdleCounter = 0;
    modes->GetCurrentMode().GetType();
    float rate = g_primitiveDrawDistanceScale + 1.0f;
    if (rate * 9999980.0f < time) {
        Blinking = false;
        animation.ClearAllAnims();
        const CAnimationEntry& anim = GetAnimFromName("DEFAULT", 0);
        CAnimInit init(6, 0, anim.BankIndex, (EAnimUpdateMode)1, false, anim.GetAnimateThingFlags(), 100);
        init.Speed = 0.0f;
        animation.SetMainAnim(init, 1, 0);
        animation.SetInterruptable(true);
    } else {
        bool update_main_anim = true;
        if (animation.IsMainAnimFinished() || animation.IsInterruptable()) {
            if (PCurrentAction.Data && !PCurrentAction.Data->IsFinished()) {
                long action_anim = PCurrentAction.Data->GetAnimationIndex();
                if (action_anim) {
                    if (!IsAnimCombinationAnim(action_anim))
                        update_main_anim = false;
                    CAnimSet* combination = animation.GetFirstCombinationAnimFromType(25);
                    if (combination)
                        combination->SetAsFinished(3);
                }
            } else if (animation.GetMainAnimType() == 7 && !animation.PeekMainAnim().IsFinished()) {
                update_main_anim = false;
            }
        }
        if (update_main_anim) {
            CTCCreatureNavigation* navigation;
            if (GetTC(TCI_NAVIGATION, &navigation) && navigation->GetNavigatorType() == FlyingNavigatorType
                && distance > DELTA_NOTIONAL_ZERO) {
                if (-DELTA_NOTIONAL_ZERO <= MovementVector.Z) {
                    if (animation.GetMainAnimType() != 9) {
                        const CAnimationEntry& fly = GetAnimFromName("STANDARD_FLY", 0);
                        animation.SetMainAnim(CAnimInit(9, 0, fly.BankIndex, (EAnimUpdateMode)1, true, fly.GetAnimateThingFlags(), 100),
                                              fly.GetTimeToBlendIn(*this), fly.GetDelayTime());
                    }
                } else if (animation.GetMainAnimType() != 10) {
                    const CAnimationEntry& glide = GetAnimFromName("STANDARD_GLIDE", 0);
                    animation.SetMainAnim(CAnimInit(10, 0, glide.BankIndex, (EAnimUpdateMode)1, true, glide.GetAnimateThingFlags(), 100),
                                          glide.GetTimeToBlendIn(*this), glide.GetDelayTime());
                }
            } else {
                modes->GetCurrentMode().AnimUpdate(animation);
            }
        }
        if ((animation.PeekMainAnim().GetFlags() & 4) && rate * 12.0f > time) {
            if (AnimExists("DEFAULT_FACE", false)) {
                const CAnimationEntry& face = GetAnimFromName("DEFAULT_FACE", 0);
                if (!animation.GetFirstCombinationAnimFromType(20))
                    animation.AddCombinationAnim(CAnimInit(20, 0, face.BankIndex, (EAnimUpdateMode)1, false, face.GetAnimateThingFlags(), 100),
                                                 0, 0, false, false);
            }
        } else {
            animation.RemoveCombinationAnimsOfType(20);
        }
        CTCLook* look;
        if (GetTC(TCI_LOOK, &look) && !look->IsInsideAnimationRange()) {
            animation.RemoveCombinationAnimsOfType(11);
            animation.RemoveCombinationAnimsOfType(12);
        }
    }
    if (!(rate * 12.0f > time) && !DemonDoor && !Oracle) {
        animation.RemoveCombinationAnimsOfType(18);
    } else {
        CTCCreatureExpression* expression;
        if (GetTC(TCI_CREATURE_EXPRESSION, &expression))
            expression->Update(animation, update);
    }
    if (!(rate * 8.0f > time) && !DemonDoor && !Oracle) {
        animation.RemoveCombinationAnimsOfType(19);
    } else {
        CTCTalk* talk;
        if (GetTC(TCI_TALK, &talk))
            talk->Update(animation, update);
    }
    if (PCurrentAction.Data && !PCurrentAction.Data->IsFinished() && PCurrentAction.Data->GetAnimationIndex())
        PCurrentAction.Data->UpdateActionAnimation();
    animation.Update(update);
}