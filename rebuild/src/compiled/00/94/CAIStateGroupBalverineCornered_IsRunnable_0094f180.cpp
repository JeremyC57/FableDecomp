// CAIStateGroup_BalverineCornered::IsRunnable() -- the cornered balverine may jump back at its best enemy (or at a live creature that
//   just hit it in melee, unblocked): the target must be alive and known to it (skipped for the melee attacker), and unless it was
//   hit in melee the target must be inside the combat type's FrontMiddleLimit (2D) and the spot one diameter behind the balverine
//   out of nav line of sight; then the landing spot next to the target (GetLandPos) must be in nav line of sight of the target.
//   On success it records LandPos, TimeOfLastJump (world frame), PLastJumpTarget and PTarget.
// Leaf: ?IsRunnable@CAIStateGroup_BalverineCornered@@MAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094F180, 754 bytes, thiscall, no arguments, plain `ret`, FPO, no SEH; one shared false exit.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?IsRunnable@CAIStateGroup_BalverineCornered@@MAE_NXZ (protected virtual);
//   vtable 0x0128C0BC slot 11 (rebuild/manifest/rtti-vtables.tsv) is its only reference.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline CThing::GetTC, interfaces 8 and 0x42) and const
//   0x004365B0 (inline CThing::PeekTC, interface 7), CTCTargetingBase vslot 29 (+0x74) GetPBestEnemyCreatureForAttack,
//   CTCEntityEvents::GetEvent<const CEventHitBy, CFilter_Bind2<CIsHitEventHitByMelee,CIsHitEventBlocked> > 0x0094F890,
//   CEventHitBase::GetHitParams 0x00810F40, CBaseIntelligentPointer::GetPItem 0x00A01B50, NCreatureAI::IsAwareOfThing 0x008645E0,
//   CThing::GetPos 0x004C73D0, C3DVector::AccurateNormalise 0x00A14440, CAIStateGroupBase::GetWorld 0x008FCFC0, CWorld vslot 12
//   GetWorldMap (result unused), CThingCreatureBase::GetCombatTypeDef 0x00666D10, CTCCreatureNavigation::IsPosNavLineOfSightTo
//   0x006CBA20 / IsPosNavLineOfSightToPos 0x006CB610, GetLandPos 0x0094F090, CWorld::GetFrame 0x0049D870,
//   CIntelligentPointer<CThingCreatureBase>::CIntelligentPointer(T*) 0x00668B60, SetPItem 0x00A01B90, ~CBaseIntelligentPointer 0x00A01C10.
// Shape: the nav pointer is left uninitialised when the creature has no navigation TC (retail has no else); the two `PX = target`
//   assignments are the out-of-line (T*) ctor + the inline operator= (GetPItem, SetPItem) + the temporary's dtor -- this TU expands
//   operator= in place past VC7.1's inline budget, hence the FORCEINLINE opt-in.
// Headers: overlay opt-ins FABLE_RETAIL_CAISTATEGROUP_BALVERINECORNERED, FABLE_RETAIL_NENTITYEVENTS_CEVENTHITBASE_GETHITPARAMS,
//   FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_FORCEINLINE.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_COMBATTYPE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_BALVERINECORNERED
#define FABLE_RETAIL_CTCENTITYEVENTS_GETEVENT_DECL
#define FABLE_RETAIL_NENTITYEVENTS_CEVENTHITBASE_GETHITPARAMS
#define FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_CTCCREATURENAVIGATION_ONEPOINTLOS
#define FABLE_RETAIL_CTCCREATURENAVIGATION_LINEOFSIGHT
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCPHYSICSBASE_DIAMETER
#define FABLE_RETAIL_JACK_STAGE1_UPDATE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CVectorMap.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_CEventHitBase.h"
#include "engine/NEntityEvents_CEventHitBy.h"
#include "engine/NEntityEvents_EventFilters.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CHitParameters.h"
#include "engine/CAIStateGroup_BalverineCornered.h"

namespace NCreatureAI {
    bool __fastcall IsAwareOfThing(const CThingAICreature* creature, const CThing* thing, bool direct);   // 0x008645E0
}

bool CAIStateGroup_BalverineCornered::IsRunnable()
{
    CTCTargetingBase* targeting;
    if (GetCreature().GetTC(TCI_TARGETING, &targeting)) {
        CThingCreatureBase* target = targeting->GetPBestEnemyCreatureForAttack();
        bool hitByMelee = false;
        CTCEntityEvents* events;
        if (GetCreature().GetTC(TCI_ENTITY_EVENTS, &events)) {
            const NEntityEvents::CEventHitBy* event;
            if (events->GetEvent(&event, CFilter_Bind2<NEntityEvents::CIsHitEventHitByMelee, NEntityEvents::CIsHitEventBlocked>(
                    NEntityEvents::CIsHitEventHitByMelee(), NEntityEvents::CIsHitEventBlocked()))) {
                CThing* attacker = event->GetHitParams().GetPThingHitting();
                if (attacker && !attacker->IsDead() && (attacker->GetTypeFlags() & 8)) {
                    target = (CThingCreatureBase*)attacker;
                    hitByMelee = true;
                }
            }
        }
        if (target && !target->IsDead() && (hitByMelee || NCreatureAI::IsAwareOfThing(&GetCreature(), target, false))) {
            C3DVector away = GetCreature().GetPos() - target->GetPos();
            away.AccurateNormalise();
            C3DVector jumpBack = away * GetCreature().GetPhysicsTC()->GetDiameter() + GetCreature().GetPos();
            GetWorld().GetWorldMap();
            const CTCCreatureNavigation* nav;
            GetCreature().PeekTC(TCI_NAVIGATION, &nav);
            C3DVector toTarget = GetCreature().GetPos() - target->GetPos();
            const CCombatTypeDef& combat = GetCreature().GetCombatTypeDef();
            if (hitByMelee || (toTarget.GetSquaredMagnitude2D() < combat.FrontMiddleLimit * combat.FrontMiddleLimit
                               && !nav->IsPosNavLineOfSightTo(jumpBack))) {
                C3DVector land = GetLandPos(*target);
                if (nav->IsPosNavLineOfSightToPos(target->GetPos(), land)) {
                    LandPos = land;
                    TimeOfLastJump = CWorld::GetFrame();
                    PLastJumpTarget = target;
                    PTarget = target;
                    return true;
                }
            }
        }
    }
    return false;
}