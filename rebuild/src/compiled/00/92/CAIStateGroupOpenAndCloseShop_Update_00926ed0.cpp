// 00926ed0 CAIStateGroup_OpenAndCloseShop::Update
// Leaf: ?Update@CAIStateGroup_OpenAndCloseShop@@UAEXXZ
// Flags:
#define FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE_FASTCALL
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_OPEN_CLOSE_SHOP_PASS29
#define FABLE_RETAIL_CTCTRESPASSABLE
#define FABLE_RETAIL_UPDATE_CURFEW_PASS29
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTCSHOPKEEPER_SHOP
#define FABLE_RETAIL_CTCDOOR
#define FABLE_RETAIL_CTCDOOR_VANDALISM
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CTHINGAICREATURE_BUILDING_GETTERS_PASS26
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING
#define FABLE_RETAIL_MINION_CRIMINAL_UPDATE_PASS24
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/fable_ai_action_enums.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_OpenAndCloseShop.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CTCDoor.h"
#include "engine/CTCTrespassable.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCShopKeeper.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/ThingAlive.h"

#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
namespace std {
template<> inline void FABLE_FASTCALL __destroy_aux(CIntelligentPointer<const CThing>* first,CIntelligentPointer<const CThing>* last,const __false_type&) { for(;first!=last;++first) _Destroy(&*first); }
template<> inline vector<CIntelligentPointer<const CThing> >::~vector() { _Destroy(this->_M_start,this->_M_finish); }
}

#ifdef _MSC_VER
#pragma optimize("",on)
#endif

bool FABLE_FASTCALL OrderByHeight(CIntelligentPointer<const CThing>, CIntelligentPointer<const CThing>);

void CAIStateGroup_OpenAndCloseShop::Update()
{
    CIntelligentPointer<CThingBuilding> shop(GetCreature().GetWorkBuilding());
    if (!IS_THING_ALIVE(shop)) {
        Terminate();
        return;
    }
    CTCTrespassable* ptc_trespassable;
    if (Lock && shop->GetTC(&ptc_trespassable))
        ptc_trespassable->SetTrespassable(true);

    {
        std::vector<CIntelligentPointer<const CThing> > doors;
        const CTCThingOwner* owner;
        if (shop->PeekTC(&owner)) {
            const std::list<CIntelligentPointer<CThing> >& objects = owner->GetObjectList();
            for (std::list<CIntelligentPointer<CThing> >::const_iterator object_it = objects.begin();
                 object_it != objects.end(); ++object_it) {
                CTCDoor* door;
                if (IS_THING_ALIVE(*object_it) && (*object_it)->GetTC(&door) && !(*object_it)->IsInLimbo())
                    doors.push_back(object_it->Get());
            }
        }
        std::sort(doors.begin(), doors.end(), OrderByHeight);
        EAIActionReturn ret = AI_ACTION_FAILED;
        C3DVector pos;
        for (int n = 0; n < doors.size() && IS_THING_ALIVE(shop); ++n) {
            if (IS_THING_ALIVE(doors[n])) {
                pos = doors[n]->GetPos();
                C3DVector front = pos + doors[n]->GetPhysicsTC()->GetRHSet().GetForward() * 1.1f;
                C3DVector back = pos - doors[n]->GetPhysicsTC()->GetRHSet().GetForward() * 1.1f;
                if (GoToInsideFace == shop->IsPointInside(front)) {
                    bool nav_route_calculated;
                    ret = AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED, front, nav_route_calculated,
                        AI_MOVEMENT_RUNNING, true, true, 0, true, 0.0f);
                    if (ret == AI_ACTION_FINISHED) break;
                } else if (GoToInsideFace == shop->IsPointInside(back)) {
                    bool nav_route_calculated;
                    ret = AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED, back, nav_route_calculated,
                        AI_MOVEMENT_RUNNING, true, true, 0, true, 0.0f);
                    if (ret == AI_ACTION_FINISHED) break;
                }
            }
        }
        if (ret == AI_ACTION_FINISHED && IS_THING_ALIVE(shop)) {
            AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED, pos, 1, 0.0027777f);
            CTCShopKeeper* tc_shopkeeper;
            if (Lock && GetCreature().GetTC(&tc_shopkeeper) && tc_shopkeeper->GetAreOtherPeopleInShop(false)) {
                Terminate();

#ifdef _MSC_VER
// Inline-budget: the shared early exit calls the vector destructor at 009273d4.
#pragma inline_depth(0)
#endif
                goto exit_shop;
#ifdef _MSC_VER
#pragma inline_depth()
#endif

            }
            shop->LockAllDoors(Lock, false);
            if (!Lock && shop->GetTC(&ptc_trespassable))
                ptc_trespassable->SetTrespassable(false);
            int start_wait = CWorld::GetFrame();
            while (Lock ? !shop->IsLocked() : !shop->IsUnlocked()) {
                if (CWorld::GetFrame() - start_wait > 75) break;
                if (AIAction_DoNothing() != AI_ACTION_IN_PROGRESS) {
                    Terminate();

#ifdef _MSC_VER
// Inline-budget: the shared early exit calls the vector destructor at 009273d4.
#pragma inline_depth(0)
#endif
                    goto exit_shop;
#ifdef _MSC_VER
#pragma inline_depth()
#endif

                }
            }
        }
        Terminate();
    }
// Inferred: both early exits destroy doors before the direct shop base destructor call.
exit_shop:
    return;
}