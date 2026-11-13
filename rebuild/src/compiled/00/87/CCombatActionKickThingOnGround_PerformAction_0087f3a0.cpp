// CCombatAction_KickThingOnGround::PerformAction() -- the kick lands: when the kicker reached MoveToPos, tell the living target it was kicked, then either launch it as a shot (kickable def with a shot) or hit / animate it, and post the kick's opinion deed
// Leaf: ?PerformAction@CCombatAction_KickThingOnGround@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x0087F3A0, 2422 bytes (`sub esp,0C4h` .. `ret` at 0x0087FD15), no EH frame, no flags.
//   CCombatAction_KickThingOnGround vtable 0x01277D3C slot 3 (rtti-vtables.tsv); Ego_r.pdb and FableWin.pdb both carry
//   ?PerformAction@CCombatAction_KickThingOnGround@@UAEXXZ. Landed siblings: Initialise 0x0087E860, GetNextPos 0x0087E770,
//   FinishAction 0x0087EBD0, the constructor 0x0087FD20.
// Flow (retail addresses):
//   1. 0x0087F3A0 refuse while the kicker is more than 0.2 (squared 0.04, pool 0x01277D30) away from MoveToPos in the ground plane,
//      more than 1.0 above / below it (fabs, 0x0087F412), or while the target is null / dead (IS_THING_ALIVE, 0x0087F425).
//   2. 0x0087F44E the target's CTCEntityEvents (interface 0x42) gets a new NEntityEvents::CEventKicked (8 bytes, vtable 0x01277B5C).
//   3. 0x0087F4AF kickable_def = target def's CKickableDef sub-def. With a ShotToAttach: is_kickable, and a creature target
//      (type flag 8) is told who attacked it. Otherwise a creature target takes a 1.0 unarmed melee hit (OnHit, vtable +0x104),
//      and a dead-creature target (type 13) plays RESPONSE_GET_KICKED_ON_GROUND when its appearance has one (its own animation
//      filter when that has the animation, else the no-group filter), on its animating TC (added when missing).
//   4. 0x0087F79D a type-5 object target has its die-off timer reset; without a kickable def the action ends here.
//   5. 0x0087F7C0 is_kickable: the shot def's class is added to the target as a TC and initialised to fly from the target along the
//      kicker's facing: length = (random(RandomRange) + RandomBase) * (BaseSpeedScaleRange * base_speed + SpeedScaleRange * speed
//      + StrengthScaleRange * strength + BaseDistanceToTravel). A hero that is not a child scales by physique level / max, by the
//      part of Movement along the kick direction over MaxRunningSpeed, and by (speed level + 1) / (max + 1). The target's mode
//      manager gets mode 0x39, a CTCRotate spins it about Z, and the shot def's PrimaryEffect is emitted 0.3 above it.
//   6. 0x0087FC74 the def's opinion deed is posted from the kicker's deed log (no faction, type set).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPos 0x004C73D0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
//   0x0040F020 (GetTC), operator new 0x00BFEA1A, CTCEntityEvents::AddEvent 0x008113C0, CDefClassBase::GetSubDef<CKickableDef>
//   0x006AE260, CThingCreatureBase::SetAttackedByCreature 0x006624F0, CHitParameters(CHitLocation*, CThing*, CThing*) 0x0087FF60,
//   CHitParameters::AddDamageWithAttribute 0x00868CA0, ~CHitParameters 0x00789358, CDefClassBase::GetSubDef<CAppearanceDef>
//   0x0042B0A2, CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, CAnimationSet::AnimExists 0x005DC2E0,
//   CAnimationSet::GetRandomAnim 0x005DCC00, CCreatureActionBase::GetWorld 0x00692940, CWorld::GetWorldSeed 0x0049D810,
//   CThing::AddTC 0x004C9D60, CTCAnimatingObject::PlayAnimation 0x007F9010, CThing::SetAsRenderedThisFrame 0x004C7590,
//   CThingObject::ResetDieOffTimer 0x00702CE0, CDefinitionManager::GetDef<CThingShotDef> 0x0078B230, CDefString::GetString
//   0x00415DD0, CThingCreatureBase::IsChild 0x00661F70, CTCHeroExperience::GetTrainableStatLevel 0x006D6520 /
//   GetTrainableStatMaxLevel 0x006D6770, GFFloatRandom 0x004978A0, NThingShot::CTCShotDefinitionBase::Init (const C3DVector&
//   overload) 0x007BF790, CTCCreatureModeManager::AddMode 0x0073AE10, CTCRotate::SetRotationInfo 0x0072A530,
//   CTCDParticleEmitter::Create 0x006E0880, CTCHeroOpinionDeedLog::PostDeed 0x006C6990. Data: GFastOneOverSqrtTable 0x013CE558
//   (GFFastOneOverSqrt inside C3DVector::Normalise, table load at 0x0087F969), vtables CEventKicked 0x01277B5C,
//   CAnimFilterNoGroup 0x0122FFDC, CAnimFilterBase 0x0122FFCC; float pool 0.04f 0x01277D30, 1.0f 0x0122DED8, 3.0f 0x0122DED4,
//   0.5f 0x0122F59C, 0.3f 0x01238170; strings 0x01277D10, 0x0123C260 "CTCAnimatingObject", 0x0123A014 "CTCRotate".
// Shape (what each spelling buys; PROVEN = a second source besides this row's grade, FITTED = the row grade only):
//   - Visible shared bodies of GetSubDef / GetDef (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY, .._GETDEF_GLOBAL_BODY) give retail's
//     in-place `dec [p+4]` handle releases; the original CThing vtable opt-in gives the +0x104 OnHit slot (existing opt-ins).
//   - The dead-creature test is ONE `&&` holding the GetPBaseDef() / CCharString / CAnimFilterNoGroup temporaries: retail's
//     conditional-destruction flags 2 / 4 / 8 in [esp+1Ch] (0x0087F5A5..0x0087F643). PROVEN by the flag bits.
//   - `{ CCharString shot_class_name = ...GetString(); shot = AddTC(shot_class_name, true, 0); }`: a named block local, retail's
//     `lea` re-address of the string and its destruction before the facing call (0x0087F7F4..0x0087F820). FITTED.
//   - `effect_pos` is a named local: retail stores the sum to its own frame words and passes their address in edx (0x0087FC50..0x0087FC5C).
//     FITTED (the unnamed temporary is 9 bytes short).
//   - `C3DVector movement = Movement; ... movement = direction * direction.Dot(movement);` and `base_speed_scale = speed_scale =
//     movement.GetMagnitude() / MaxRunningSpeed;` reuse the copies as retail's frame words do. FITTED.
//   - Two shared inline texts decide the x87 operand order of the projection (Dot 0x0087F9B6..0x0087F9D2, magnitude
//     0x0087FA0A..0x0087FA2E), both opt-in, body only:
//     FABLE_RETAIL_C3DVECTOR_MAGNITUDE_XY_THEN_Z `float squared=(X*X+Y*Y)+Z*Z;return (float)sqrt(squared);` is PROVEN by size
//     (26i/66b at /Od = FableWin.pdb ?GetMagnitude@C3DVector@@QBEMXZ) plus the grade; FABLE_RETAIL_C3DVECTOR_DOT_XY_THEN_Z
//     `float dot=(X*other.X+Y*other.Y);dot+=Z*other.Z;return dot;` is FITTED (23i/60b v FableWin 23i/59b; no natural text of 26
//     tried is 59 bytes; the neighbouring texts without the parentheses or with the default one-line sum grade DIFFER).
//   - x87 only: fabs 0x0087F412, fild 0x0087F8BF / 0x0087F8D4 / 0x0087FA49 / 0x0087FA64, fsqrt 0x0087FA2E. No fistp, no ftol call,
//     no SSE: plain C++ throughout, nothing from the shared float-to-long helpers.
//   - OPINION_POST_DEED_TYPE_ALL = 7 is the spelling of the landed 0x006D00A0 (CTCWife::Murdered) and 0x00ECB0E0 (script enum
//     list: RECIPIENT 1 | WITNESSES 2 | VILLAGE 4); the enumerator list is in no PDB name set on this machine.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTION_KICKTHINGONGROUND
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CHILD
#define FABLE_RETAIL_CTHINGDEADCREATURE_ANIMATION
#define FABLE_RETAIL_CTHINGDEADCREATURE_GETANIMATIONFILTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_DOT_XY_THEN_Z
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_XY_THEN_Z
#define FABLE_RETAIL_C3DVECTOR_TO2D
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CANIMATIONSET_ANIMEXISTS
#define FABLE_RETAIL_CANIMATIONSET_ANIMS
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CKICKABLEDEF
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCANIMATINGOBJECT_PLAYANIMATION
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_LOCATIONCTOR
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETATTACKEDBYCREATURE
#define FABLE_RETAIL_CTHINGOBJECT_RESETDIEOFFTIMER
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT_POSITION
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHINGSHOTDEF_DEFSTRING
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingObject.h"
#include "engine/CThingDeadCreature.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAppearanceDef.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/CKickableDef.h"
#include "engine/CThingShotDef.h"
#include "engine/CHitParameters.h"
#include "engine/NEntityEvents_CEventKicked.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CTCAnimatingObject.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCRotate.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
#include "engine/GFFloatRandom.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCombatAction_KickThingOnGround.h"
#include <math.h>

enum EOpinionPostDeedType { OPINION_POST_DEED_TYPE_ALL = 7 };
class CMeleeCombatAbilityDef;

void CCombatAction_KickThingOnGround::PerformAction()
{
    if ((PCreature->GetPos() - MoveToPos).ToC2DVector().GetSquaredMagnitude() > 0.04f)
        return;
    if (fabs(PCreature->GetPos().Z - MoveToPos.Z) > 1.0f)
        return;
    if (!IS_THING_ALIVE(PTarget))
        return;
    CTCEntityEvents* entity_events;
    if (PTarget->GetTC(&entity_events))
        entity_events->AddEvent(new NEntityEvents::CEventKicked);
    bool is_kickable = false;
    CDefPointer<const CKickableDef> kickable_def;
    if (PTarget->GetPBaseDef()->GetSubDef(&kickable_def) && kickable_def->ShotToAttach != 0) {
        is_kickable = true;
        if (PTarget->GetTypeFlags() & 8)
            static_cast<CThingCreatureBase*>(PTarget.Get())->SetAttackedByCreature(PCreature.Get());
    } else if (PTarget->GetTypeFlags() & 8) {
        CHitParameters hit_params(0, PCreature.Get(), 0);
        hit_params.AddDamageWithAttribute(DAMAGE_MELEE_UNARMED, 1.0f);
        static_cast<CThingGameObject*>(PTarget.Get())->OnHit(hit_params);
    } else if (PTarget->GetType() == 13) {
        CDefPointer<const CAppearanceDef> appearance;
        if (PTarget->GetPBaseDef()->GetSubDef(&appearance) &&
            appearance->Animation.AnimExists("RESPONSE_GET_KICKED_ON_GROUND", CAnimFilterNoGroup())) {
            CThingDeadCreature* dead_creature = static_cast<CThingDeadCreature*>(PTarget.Get());
            CAnimFilterNoGroup no_group;
            const CAnimFilterBase* filter = &no_group;
            if (appearance->Animation.AnimExists("RESPONSE_GET_KICKED_ON_GROUND", dead_creature->GetAnimationFilter()))
                filter = &dead_creature->GetAnimationFilter();
            const CAnimationEntry& anim = appearance->Animation.GetRandomAnim("RESPONSE_GET_KICKED_ON_GROUND",
                                                                              *GetWorld().GetWorldSeed(), *filter, -1);
            CTCAnimatingObject* animating;
            if (!PTarget->GetTC(&animating))
                animating = static_cast<CTCAnimatingObject*>(PTarget->AddTC("CTCAnimatingObject", false, 0));
            animating->PlayAnimation(anim.BankIndex, 0, false);
            PTarget->SetAsRenderedThisFrame(2);
        }
    }
    if (PTarget->GetType() == 5)
        static_cast<CThingObject*>(PTarget.Get())->ResetDieOffTimer();
    if (kickable_def.IsNull())
        return;
    if (is_kickable) {
        CDefPointer<const CThingShotDef> shot_def;
        GetWorld().PeekDefinitionManager().GetDef(kickable_def->ShotToAttach, &shot_def);
        NThingShot::CTCShotDefinitionBase* shot;
        {
            CCharString shot_class_name = shot_def->ShotDefinitionClassName.GetString();
            shot = static_cast<NThingShot::CTCShotDefinitionBase*>(PTarget->AddTC(shot_class_name, true, 0));
        }
        C3DVector facing = PCreature->GetPhysicsTC()->GetFacingVector();
        float strength_scale = 0.0f;
        float speed_scale = 0.0f;
        float base_speed_scale = 0.0f;
        CTCHeroExperience* experience;
        if (PCreature->GetTC(&experience) && !PCreature->IsChild()) {
            strength_scale = (float)experience->GetTrainableStatLevel(HERO_STAT_STRENGTH_PHYSIQUE) /
                             (float)experience->GetTrainableStatMaxLevel(HERO_STAT_STRENGTH_PHYSIQUE);
            C3DVector movement = Movement;
            C3DVector direction = PTarget->GetPos() - PCreature->GetPos();
            direction.Normalise();
            movement = direction * direction.Dot(movement);
            base_speed_scale = speed_scale = movement.GetMagnitude() / MaxRunningSpeed;
            speed_scale *= ((float)experience->GetTrainableStatLevel(HERO_STAT_SKILL_SPEED) + 1.0f) /
                           ((float)experience->GetTrainableStatMaxLevel(HERO_STAT_SKILL_SPEED) + 1.0f);
        }
        C3DVector velocity = facing * (GFFloatRandom(kickable_def->RandomRange, GetWorld().GetWorldSeed()) + kickable_def->RandomBase) *
            (kickable_def->BaseSpeedScaleRange * base_speed_scale + kickable_def->SpeedScaleRange * speed_scale +
             kickable_def->StrengthScaleRange * strength_scale + kickable_def->BaseDistanceToTravel);
        C3DVector start = velocity + PTarget->GetPos();
        shot->Init(shot_def, PCreature.Get(), 0, start, 50.0f, 1.0f, 0.0f, CDefPointer<const CMeleeCombatAbilityDef>());
        CTCCreatureModeManager* mode_manager;
        if (PTarget->GetTC(&mode_manager))
            mode_manager->AddMode(0x39);
        CTCRotate* rotate = static_cast<CTCRotate*>(PTarget->AddTC("CTCRotate", false, 0));
        if (rotate) {
            rotate->SetRotating(true);
            rotate->SetRotationInfo(C3DVector(0.0f, 0.0f, 1.0f), 0.15f, 5.0f);
        }
        C3DVector effect_pos = PTarget->GetPos() + C3DVector(0.0f, 0.0f, 0.3f);
        CTCDParticleEmitter::Create(shot_def->PrimaryEffect, effect_pos, false);
    }
    if (kickable_def->OpinionDeedType != OPINION_DEED_TYPE_NONE || kickable_def->OpinionDeedTypeToEnemies != OPINION_DEED_TYPE_NONE) {
        CTCHeroOpinionDeedLog* deed_log;
        if (PCreature->GetTC(&deed_log)) {
            if (kickable_def->OpinionDeedFaction == 0 && kickable_def->OpinionDeedType != OPINION_DEED_TYPE_NONE)
                deed_log->PostDeed(kickable_def->OpinionDeedType, PCreature->GetPos(), PTarget.Get(), 0, OPINION_POST_DEED_TYPE_ALL,
                                   false, false, 0, false, 0);
        }
    }
}