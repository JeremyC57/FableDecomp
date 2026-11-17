// CCreatureAction_GenericFireProjectileWeapon::PerformAction() -- with the creature alive: make a SIMPLE_NOISE, unload the carried weapon, play its PROJECTILE_WEAPON_FIRE animation, create the weapon's shot at the target and post a fired-shot event on the creature
// Leaf: ?PerformAction@CCreatureAction_GenericFireProjectileWeapon@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) 0x008BF280, 638 bytes, thiscall `ret` (vtable 0x0127F384 slot 3). Grade: RELOCATION_MATCH (e06 egod lane,
//   first compiling grade of the port).
// Names: Ego_r.pdb ?PerformAction@CCreatureAction_GenericFireProjectileWeapon@@UAEXXZ; Ego_d combat_action_bandit.obj locals and scopes
//   verbatim: block {ptc_weapon (CTCWeapon*), pcarried_thing (CThing*), ptc_carry (const CTCCarrying*)} { shot_damage_multiplier (float),
//   global_index, ptc_animating_object, shot_accuracy_percentage (float) { pshot { pTCEvents } } }. The old draft's residues (weapon in
//   esi, the float sharing the int slot) came from its local models and missing named float locals.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CThingCreatureBase::MakeNoise 0x00662110, PeekTC<const CTCCarrying> (const LowerBound 0x004365B0; the result is not tested, so
//   the not-present path reads the uninitialised ptc_carry, 0x008BF319), GetTC<CTCWeapon / CTCAnimatingObject / CTCEntityEvents>
//   (LowerBound 0x0040F020), CTCCarrying::GetThingInPrimarySlot const 0x006A1440, CTCWeapon::SetProjectileWeaponLoaded 0x005DAC50 /
//   GetCreatedShotDefGlobalIndex const 0x005DA750, CTCAnimatingObject::PlayAnimation(const CCharString&, long, bool) 0x007F90B0,
//   CThingCreatureBase::GetShotAccuracyPercentage const 0x00663B10, NThingShot::CreateShot 0x007C0AD0 (__fastcall; Ego_d parameter
//   names; the zeroed CDefPointer temporary at 0x008BF3FE is the defaulted pability_def), CTCGraphicAppearance::
//   SetForceResendEveryRenderFrame (inline, `or dl,2` on +0x59), operator new 0x00BFEA1A, NEntityEvents::CEventFiredShot(CThing&)
//   0x008BFE20 (same TU, called), CTCEntityEvents::AddEvent 0x008113C0.
// Headers: as the landed same-TU FinishAction 0x008BF500 plus hdr/ opt-in FABLE_RETAIL_CTCWEAPON_GETCREATEDSHOTDEFGLOBALINDEX and the new
//   hdr/engine/NEntityEvents_CEventFiredShot.h; existing CTCANIMATINGOBJECT_PLAYANIMATION(_NAME), CTCENTITYEVENTS(_ADDEVENT),
//   CTCGRAPHICAPPEARANCE_MOVEMENT + _FORCERESEND, CTHINGCREATUREBASE_GETSHOTACCURACYPERCENTAGE. The CreateShot default arguments
//   (pability_def, pos, rhs) are FITTED: the call is the same with them written out.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETSHOTACCURACYPERCENTAGE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCREATUREACTION_GENERICFIREPROJECTILEWEAPON
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_SETLOADED
#define FABLE_RETAIL_CTCWEAPON_GETCREATEDSHOTDEFGLOBALINDEX
#define FABLE_RETAIL_CTCANIMATINGOBJECT_PLAYANIMATION
#define FABLE_RETAIL_CTCANIMATINGOBJECT_PLAYANIMATION_NAME
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FORCERESEND
#include "rebuild_abi.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/ThingAlive.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCAnimatingObject.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NEntityEvents_CEventFiredShot.h"
#include "engine/CCreatureAction_GenericFireProjectileWeapon.h"
class CRightHandedSet;
class CMeleeCombatAbilityDef;
namespace NThingShot {
CThing* __fastcall CreateShot(long def_index, CThing* pshooter, CThing* pweapon, const CThing* ptarget, float accuracy_percentage,
    float shot_damage_multiplier, float shot_buildup_time,
    const CDefPointer<const CMeleeCombatAbilityDef>& pability_def = CDefPointer<const CMeleeCombatAbilityDef>(),
    const C3DVector* pos = 0, const CRightHandedSet* rhs = 0);         // 0x007C0AD0
}

void CCreatureAction_GenericFireProjectileWeapon::PerformAction()
{
    if (IS_THING_ALIVE(PCreature)) {
        PCreature->MakeNoise("SIMPLE_NOISE");
        CTCWeapon* ptc_weapon = 0;
        const CTCCarrying* ptc_carry;
        PCreature->PeekTC(&ptc_carry);
        CThing* pcarried_thing = ptc_carry->GetThingInPrimarySlot();
        if (IS_THING_ALIVE(pcarried_thing)) {
            if (pcarried_thing->GetTC(&ptc_weapon))
                ptc_weapon->SetProjectileWeaponLoaded(false, 0);
            CTCAnimatingObject* ptc_animating_object;
            if (pcarried_thing->GetTC(&ptc_animating_object))
                ptc_animating_object->PlayAnimation("PROJECTILE_WEAPON_FIRE", 0, false);
            float shot_accuracy_percentage = (float)PCreature->GetShotAccuracyPercentage();
            long global_index = ptc_weapon->GetCreatedShotDefGlobalIndex();
            float shot_damage_multiplier = 1.0f;
            if (global_index) {
                CThing* pshot = NThingShot::CreateShot(global_index, PCreature.Get(), pcarried_thing, PTarget.Get(),
                                                       shot_accuracy_percentage, shot_damage_multiplier, 1.0f);
                if (IS_THING_ALIVE(pshot)) {
                    pshot->GetTCGraphicAppearance()->SetForceResendEveryRenderFrame(true);
                    CTCEntityEvents* pTCEvents;
                    if (PCreature->GetTC(&pTCEvents))
                        pTCEvents->AddEvent(new NEntityEvents::CEventFiredShot(*PTarget));
                }
            }
        }
    }
}
