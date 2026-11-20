// CCombatActionBase::OnAnimationLoaded() -- once the action's animation is loaded: set up the strike events / interrupts / action times; tell a live target's hit-notification component who is attacking; record the animation's end handedness on the creature; count a boss's strikes (strike actions) or reset the combo chain (others); face a live target at the start; then, for a melee-knockdown animation, give a live target a flourish-target component and, from the melee ability's knockdown-effects def, start a scripted spline camera on a random valid combat spline and blend in the environment theme
// Leaf: ?OnAnimationLoaded@CCombatActionBase@@MAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00859EF0, 1196 bytes (plain `ret`). Ego_r.pdb ?OnAnimationLoaded@CCombatActionBase@@MAEXXZ.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCREATUREACTIONBASE_PEEKTHINGSEARCHTOOLS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_LIGHTINGFLASH
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETHANDEDNESS
#define FABLE_RETAIL_CANIMATIONENTRY_GETCOMPONENT
#define FABLE_RETAIL_CANIMATIONENTRY_GETCOMPONENT_BODY
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_ANIMCOMPONENT
#define FABLE_RETAIL_CTCBOSS_INCREMENTSTRIKES
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_RESETCOMBO
#define FABLE_RETAIL_CTCFLOURISHTARGET_SETTERS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD
#define FABLE_RETAIL_GFRANDOM_OUTOFLINE
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_SETGAMESCRIPTEDMODE
#define FABLE_RETAIL_NCAMERAMODES_CSCRIPTMODULEBASE_SETTERS
#define FABLE_RETAIL_NCAMERAMODES_CSCRIPTMODULESPLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CThingSearchTools.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefClassBase.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimComponentCombatHandedness.h"
#include "engine/CAnimComponentCombatMisc.h"
#include "engine/CTCBoss.h"
#include "engine/CTCCombat.h"
#include "engine/CTCFlourishTarget.h"
#include "engine/CThingSearchTools.h"
#include "engine/GFRandom.h"
#include "engine/NCameraModes_CScriptModuleBase.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"

class CTCCreatureHitNotification : public CTCBase {                   // no engine header (header debt); Ego_r.pdb names
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CREATURE_HIT_NOTIFICATION; }
    void SetHitNotification(const CThing& attacker);                  // 0x008651B0 ?SetHitNotification@CTCCreatureHitNotification@@QAEXABVCThing@@@Z
};

class CMeleeCombatKnockdownEffects : public CParentDefClassBase { public:   // RTTI : CParentDefClassBase; Ego_r.pdb members (retail offsets)
    long BlendInFrames;                                     // +0x3c
    long BlendOutFrames;                                    // +0x40
    long AttackerLightingChannel;                           // +0x44
    long TargetLightingChannel;                             // +0x48
    long EnvironmentTheme;                                  // +0x4c
    bool CameraFOVChange;                                   // +0x50
    bool CameraPan;                                         // +0x51
    float AttackerAndTargetSpeedMultiplier;                 // +0x54
    bool PauseEntities;                                     // +0x58
    bool NonAttackingEntities;                              // +0x59
    bool AttackerInvulnerable;                              // +0x5a
};
class CMeleeCombatAbilityDef : public CParentDefClassBase { public:   // RTTI : CParentDefClassBase
    unsigned char _pad3c[0x60 - 0x3c];                      // FlourishAnimName .. ValidWeaponClasses, Decapitate
    long KnockDownEffects;                                  // +0x60
};

namespace {
class CThingFilter_IsValidCombatSpline {                     // Ego_r.pdb `anonymous namespace'::CThingFilter_IsValidCombatSpline (8 bytes)
public:
    CThingFilter_IsValidCombatSpline(long anim_index, CThing& thing) : AnimIndex(anim_index), Thing(&thing) { }
    bool operator()(const CThing& thing) const;             // 0x0085AFD0
private:
    long AnimIndex;                                         // +0x00
    CThing* Thing;                                          // +0x04
};
}

// Fidelity: same-TU strong definition of 0x00859EC0 (retail TU-mate)
// The landed body differs only in one member spelling: HasPerformAction (+0xDE, the MELEE header) for the landed local model's m_keepActionTime (NAME_FIXES.tsv).
void CCombatActionBase::SetUpCombatAnimationNotes()
{
    SetupCombatStrikeEvents();
    SetupCombatInterrupts();
    if (StrikeEvents.begin() != StrikeEvents.end() && !HasPerformAction)
        ActionTime = -1;
}

void CCombatActionBase::OnAnimationLoaded()
{
    SetUpCombatAnimationNotes();
    InitialiseActionTimes();
    CTCCreatureHitNotification* ptc_hit_not;
    if (IsStrikeAction() && IS_THING_ALIVE(PTarget) && PTarget->GetTC(&ptc_hit_not))
        ptc_hit_not->SetHitNotification(*PCreature);
    const CAnimationEntry* panim = PAnimEntry.Get();
    const CAnimComponentCombatHandedness* phand;
    panim->GetComponent(&phand);
    PCreature->SetLastActionHandedness((ECombatAnimationHandedness)phand->EndHandedness);
    if (IsStrikeAction()) {
        CTCBoss* ptc_boss;
        if (PCreature->GetTC(&ptc_boss))
            ptc_boss->IncrementStrikesSinceHitHero();
    } else {
        CTCCombat* tc_combat;
        if (PCreature->GetTC(&tc_combat))
            tc_combat->ResetComboChainPos();
    }
    if (IsToFaceTargetAtStart() && IS_THING_ALIVE(PTarget) && PCreature.GetPItem() != PTarget.GetPItem()) {
        C3DVector direction = PTarget->GetPos() - PCreature->GetPos();
        PCreature->GetPhysicsTC()->SetFacingAngleXY(direction.GetAngleXY());
    }
    UpdateFacingDirection();
    const CAnimationEntry* combat_anim = PAnimEntry.Get();
    const CAnimComponentCombatMisc* pcombat;
    combat_anim->GetComponent(&pcombat);
    if (pcombat->MeleeKnockdown) {
        CTCFlourishTarget* ptc_flourish_target = NULL;
        if (IS_THING_ALIVE(PTarget)) {
            PTarget->RemoveTC(TCI_FLOURISH_TARGET);
            ptc_flourish_target = (CTCFlourishTarget*)PTarget->AddTC("CTCFlourishTarget", true, NULL);
            ptc_flourish_target->SetAttacker(*PCreature);
        }
        if (!PMeleeAbilityDef.IsNull() && PMeleeAbilityDef->KnockDownEffects >= 0) {
            CDefPointer<const CMeleeCombatKnockdownEffects> pkofxdef;
            PeekDefinitionManager().GetDef(PMeleeAbilityDef->KnockDownEffects, &pkofxdef);
            if (pkofxdef->CameraPan && !PCreature->HasTC(TCI_BULLET_TIME)) {
                CGameCameraManager* pcam_man;
                GetWorld().GetGameCamera()->GetCameraManager(&pcam_man);
                std::vector<CThing*> things;
                PeekThingSearchTools().GetAllThings<CThing>(CThingFilter_IsValidCombatSpline(GetAnimationIndex(), *PCreature), things);
                if (!things.empty() && IS_THING_ALIVE(PTarget)) {
                    CThing* pcamera_point = things[GFRandom(things.size(), GetWorld().GetWorldSeed())];
                    NCameraModes::CScriptModuleSpline* pspline = new NCameraModes::CScriptModuleSpline(0, -1);
                    pspline->SetPCameraPoint(*pcamera_point);
                    pspline->SetPThingOrigin(*PCreature);
                    pcam_man->SetGameScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(pspline), true);
                    PCameraPoint = pcamera_point;
                    if (ptc_flourish_target) {
                        ptc_flourish_target->SetAsCullAllYAxisMovement();
                        ptc_flourish_target->SetAsCullNegativeXAxisMovement();
                    }
                }
            }
            if (pkofxdef->EnvironmentTheme > 0)
                GetWorld().GetEnvironment().TransitionToThemeAllChannels(2, pkofxdef->EnvironmentTheme, (float)pkofxdef->BlendInFrames);
        }
    }
}
