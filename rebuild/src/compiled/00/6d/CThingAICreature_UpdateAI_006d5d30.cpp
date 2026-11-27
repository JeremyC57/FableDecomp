// 006d5d30 CThingAICreature::UpdateAI
// Leaf: ?UpdateAI@CThingAICreature@@IAE_N_NABVCBrainOutput@@AAVC3DVector@@@Z
// Flags:
#define FABLE_RETAIL_UPDATEAI_VECTOR_SUB_PASS29
#define FABLE_RETAIL_AI_CREATURE_UPDATE_PASS29
#define FABLE_RETAIL_CLOOKDEF
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CBRAINOUTPUT_ONLY
#define FABLE_RETAIL_CBRAINOUTPUT_ACTION_CALLED_PASS24
#define FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27
#define FABLE_RETAIL_CAIBRAIN
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETCURRENTACTION_COUNTED
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_DEBUGRENDER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CANIMSET_TYPE
#define FABLE_RETAIL_CTCANIMATIONCOMPLEX_FULL
#define FABLE_RETAIL_CANIMATETHING_MAINANIM
#define FABLE_RETAIL_CANIMATETHING_UPDATEANIMATION
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR_SQUARED_MAGNITUDE_ORIGINAL_EXPR_PASS23
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_PASS22_STATICFOCAL
#include "engine/EMovementType.h"
#include "engine/CThingAICreature.h"
#include "engine/campfire_types.h"
#include "engine/CAIBrain.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCLook.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCombatManager.h"

#include "engine/CTCAnimationComplex.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/ThingAlive.h"
#include "engine/ECreatureAnim.h"
namespace NGlobalConsole { extern bool EnableUpdateAI; }
namespace {
bool FABLE_FASTCALL IsFacingCorrectDirectionForMovementToPos(const CThingAICreature& creature,const CBrainOutput& output,const CTCLook& tc_look,const C3DVector& pos) {
    const CTCAnimationComplex* ptc_anim;
    creature.PeekTC(&ptc_anim);
    if(ptc_anim->PeekAnimation().GetMainAnimType()==CREATURE_ANIM_TURNING)return false;
    return true;
}
}
inline ETCInterfaceType FABLE_FASTCALL CTCLook::StatGetInterfaceType(){return TCI_LOOK;}
inline bool CBrainOutput::IsPumpCombatActions() const {return (Types&(1UL<<5))!=0;}
inline bool CBrainOutput::IsMoveToPos() const {return (Types&1)!=0;}
inline bool CBrainOutput::IsPerformAction() const {return (Types&(1UL<<3))!=0;}
inline const C3DVector& CBrainOutput::PeekMoveToPos() const {return MoveToPos;}
inline const CNavigationInput& CBrainOutput::PeekNavigationInput() const {return NavigationInput;}
inline const CCountedPointer<CCreatureActionBase>& CBrainOutput::GetAction() const {return PAction;}

// These CBrainOutput accessors and UpdateAITurning are in the original thing_creature_ai.obj.
// Retail 0x006D5B50 reads desired_movement_vector and gives LookAtPos its own look_at_pos.
inline bool CBrainOutput::IsLookAtPos() const { return (Types & 2) != 0; }
inline bool CBrainOutput::IsLookAtThing() const { return (Types & 4) != 0; }
inline bool CBrainOutput::IsFaceMovementDirection() const { return (Types & 16) != 0; }
inline const C3DVector& CBrainOutput::PeekLookAtPos() const { return LookAtPos; }
inline const CThing* CBrainOutput::PeekLookAtThing() const { return LookAtThing.Get(); }
inline long CBrainOutput::GetLookFlags() const { return LookFlags; }

// Strong same-TU owner at 0x006D5B50. The caller sees that the movement reference does not escape.
void CThingAICreature::UpdateAITurning(const C3DVector& desired_movement_vector)
{
    C3DVector look_at_pos;
    bool is_to_look = false;
    if (PCurrentBrainOutput->IsLookAtPos()) {
        is_to_look = true;
        look_at_pos = PCurrentBrainOutput->PeekLookAtPos();
    } else if (PCurrentBrainOutput->IsLookAtThing() &&
               IS_THING_ALIVE(PCurrentBrainOutput->PeekLookAtThing())) {
        is_to_look = true;
        if (PCurrentBrainOutput->PeekLookAtThing()->GetTCGraphicAppearance()) {
            if (IsInMelee()) {
                look_at_pos = PCurrentBrainOutput->PeekLookAtThing()->GetTCGraphicAppearance()->GetStaticFocalPos();
            } else {
                look_at_pos = PCurrentBrainOutput->PeekLookAtThing()->GetTCGraphicAppearance()->GetFocalPos(1.0f);
            }
        } else {
            look_at_pos = PCurrentBrainOutput->PeekLookAtThing()->GetPos();
        }
    } else if (PCurrentBrainOutput->IsFaceMovementDirection() &&
               desired_movement_vector.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO) {
        is_to_look = true;
        look_at_pos = GetPos() + desired_movement_vector;
    }
    if (is_to_look) {
        CTCLook* ptc_look;
        GetTC(&ptc_look);
        ptc_look->LookAtPos(look_at_pos, PCurrentBrainOutput->GetLookFlags());
    }
}

bool CThingAICreature::UpdateAI(bool brain_updated,const CBrainOutput& brain_output,C3DVector& to_pos)
{
    bool has_ai_pos=false;
    if (NGlobalConsole::EnableUpdateAI) {
        CTCCreatureNavigation* pnav;
        GetTC(&pnav);
        CTCLook* ptc_look;
        GetTC(&ptc_look);
        C3DVector movement_vector(0,0,0);
        if(IsInMeleeAsAttacker() && ((brain_updated && brain_output.IsPumpCombatActions()) ||
           (!brain_updated && PCurrentBrainOutput.Get() && PCurrentBrainOutput->IsPumpCombatActions()))) {
            CCombatActionBase* action=GetCombatManager().GetNextActionForCreature(*this);
            if(IsInMeleeAsAttacker()) {
                bool action_plugged_in=false;
                if(action && !IsPerformingAnyAction() && SetCurrentAction(*action)) action_plugged_in=true;
                GetCombatManager().SetCreatureAsAttemptedToPerformedAction(*this,action_plugged_in);
            }
        }
        if(brain_updated) {
            PCurrentBrainOutput.Reset(new CBrainOutput(brain_output));
            pnav->SetGotNavigationTarget(false);
            if(brain_output.IsMoveToPos()) {
                C3DVector move_to_pos(brain_output.PeekMoveToPos());
                movement_vector = move_to_pos - GetPos();
                if(IsFacingCorrectDirectionForMovementToPos(*this,brain_output,*ptc_look,move_to_pos)) {
                    has_ai_pos=true;
                    to_pos=move_to_pos;
                    pnav->SetGotNavigationTarget(true);
                    pnav->SetNavigationTarget(brain_output.PeekNavigationInput());
                }
            }
            UpdateAITurning(movement_vector);
            if(brain_output.IsPerformAction()) SetCurrentAction(brain_output.GetAction());
        } else if(PCurrentBrainOutput.Get() && !PBrain->HasStateGroupUpdated()) {
            if(PCurrentBrainOutput->IsMoveToPos() && pnav->HasGotNavigationTarget()) {
                CTCCreatureNavigation::CCreatureNavOutput nav_out=pnav->GetNextNavPosOnRoute(PCurrentBrainOutput->PeekNavigationInput(),false,true,true,false,false);
                if(nav_out.NavResult==NAV_MOVING_TO_TARGET) {
                    if(nav_out.MovementVector.IsNonZero()) {
                        if(IsFacingCorrectDirectionForMovementToPos(*this,*PCurrentBrainOutput,*ptc_look,GetPos()+nav_out.MovementVector)) {
                            has_ai_pos=true;
                            to_pos=PeekPos()+nav_out.MovementVector;
                        }
                        movement_vector=nav_out.MovementVector;
                    }
                } else pnav->SetGotNavigationTarget(false);
            }
            UpdateAITurning(movement_vector);
        }
    }
    return has_ai_pos;
}