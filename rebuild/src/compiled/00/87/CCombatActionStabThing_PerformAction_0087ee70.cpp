// 0087ee70 CCombatAction_StabThing::PerformAction
// Flags: none
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
// Leaf: ?PerformAction@CCombatAction_StabThing@@UAEXXZ
#define FABLE_RETAIL_STABTHING_PASS29
#define FABLE_RETAIL_CCOMBATACTION_STABTHING_VIRTUAL_MODEL
#define FABLE_RETAIL_CCREATUREACTION_STABBEDTODEATH_VIRTUAL_MODEL
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_NENTITYEVENTS_BASE_ORIGINAL_PASS26
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
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
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CAPPEARANCEDEF_SIZE_BODY   // NF357: GetSubDef<CAppearanceDef> pick-any copy = size-mode owner 0x0042B0A2 (scaffolding, not retail)
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD   // NF346: GetDef<T> copies = retail (the re-reading SetDynamicCast, owner 0x0066C340; as 0x0066B370)
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_TO2D
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CANIMATIONSET_ANIMEXISTS
#define FABLE_RETAIL_CANIMATIONSET_ANIMS
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCANIMATINGOBJECT_PLAYANIMATION
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCPHYSICSBASE
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
// Inline-budget: both filter vtables use a 29-byte deleting destructor with pop ecx.
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
#include "engine/CAnimFilterNoGroup.h"
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#include "engine/CHitParameters.h"
#include "engine/NEntityEvents_CEventKillLocked.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CTCAnimatingObject.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCombatAction_StabThing.h"
#include <math.h>


#include "engine/CCreatureAction_StabbedToDeath.h"
#include "engine/CTCEnemy.h"
#include "engine/EOpinionPostDeedType.h"
void CCombatAction_StabThing::PerformAction()
{
    if (!IS_THING_ALIVE(PTarget))
        return;
    if (PTarget->GetTypeFlags() & 8) {
        CThingCreatureBase& target_creature = static_cast<CThingCreatureBase&>(*PTarget);
        CTCEntityEvents* ptc_ent;
        target_creature.GetTC(&ptc_ent);
        ptc_ent->AddEvent(new NEntityEvents::CEventKillLocked(PCreature.Get()));
        target_creature.SetCurrentAction(CCreatureAction_StabbedToDeath(target_creature));
    } else if (PTarget->GetType() == 13) {
        CDefPointer<const CAppearanceDef> pappearance_def;
        if (PTarget->GetPBaseDef()->GetSubDef(&pappearance_def) &&
                pappearance_def->Animation.AnimExists("STABBED_TO_DEAD", CAnimFilterNoGroup())) {
            CCharString animName;
            CThingDeadCreature* p_dead_creature = static_cast<CThingDeadCreature*>(PTarget.Get());
            CAnimFilterNoGroup filter_no_group;
            const CAnimFilterBase* pfilter = &filter_no_group;
            if (pappearance_def->Animation.AnimExists("STABBED_ALREADY_DEAD", p_dead_creature->GetAnimationFilter())) {
                animName = "STABBED_ALREADY_DEAD";
                pfilter = &p_dead_creature->GetAnimationFilter();
            } else if (pappearance_def->Animation.AnimExists("STABBED_TO_DEAD", p_dead_creature->GetAnimationFilter())) {
                animName = "STABBED_TO_DEAD";
                pfilter = &p_dead_creature->GetAnimationFilter();
            } else if (pappearance_def->Animation.AnimExists("STABBED_TO_DEAD", CAnimFilterNoGroup())) {
                animName = "STABBED_ALREADY_DEAD";
            } else {
                animName = "STABBED_TO_DEAD";
            }
            const CAnimationEntry& anim_entry = pappearance_def->Animation.GetRandomAnim(animName,*GetWorld().GetWorldSeed(),*pfilter,-1);
            CTCAnimatingObject* ptc_anim_obj;
            if (!PTarget->GetTC(&ptc_anim_obj))
                ptc_anim_obj = static_cast<CTCAnimatingObject*>(PTarget->AddTC("CTCAnimatingObject",false,0));
            ptc_anim_obj->PlayAnimation(anim_entry.BankIndex,0,false);
            PTarget->SetAsRenderedThisFrame(2);
        }
    }
    CTCHeroOpinionDeedLog* ptc_deedlog;
    if (PCreature->GetTC(&ptc_deedlog)) {
        const CTCEnemy* ptc_enemy;
        if (PTarget->PeekTC(&ptc_enemy) && ptc_enemy->GetPFaction()) {
            ptc_deedlog->PostDeed(OPINION_DEED_TYPE_EXPRESSION_EVIL_LAUGH,PCreature->GetPos(),PTarget.Get(),0,OPINION_POST_DEED_TYPE_ALL,false,false,ptc_enemy->GetPFaction(),false,0);
            ptc_deedlog->PostDeed(OPINION_DEED_TYPE_KILL_BAD_GUY,PCreature->GetPos(),PTarget.Get(),0,OPINION_POST_DEED_TYPE_ALL,false,false,ptc_enemy->GetPFaction(),true,0);
        } else
            ptc_deedlog->PostDeed(OPINION_DEED_TYPE_EXPRESSION_EVIL_LAUGH,PCreature->GetPos(),PTarget.Get(),0,OPINION_POST_DEED_TYPE_ALL,false,false,0,false,0);
    }
}