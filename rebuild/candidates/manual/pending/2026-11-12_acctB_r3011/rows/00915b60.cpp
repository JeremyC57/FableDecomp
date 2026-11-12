// CAIStateGroup_MoveGoodsBase::PlaceGoods(CIntelligentPointer<CThing> destination, float distance, std::vector<const CCharString*> goods_names, const CAIStateGroupBase::CAIFilterBase& filter, const CThing* route_thing) -- carry the goods to a free action point of the destination crate stack and put them down
// Grade: RELOCATION_MATCH (q.py, VC7.1 RTM 13.10.3077)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00915B60, 1931 bytes (`ret 20h` at 0x009162E8, int3 to 0x009162F0 Cleanup), thiscall, esp frame
//   (sub esp,100h), no EH frame (the by-value vector is freed with free 0x00BFEA14 and the by-value destination pointer destroyed with
//   ~CBaseIntelligentPointer 0x00A01C10 at every return). Name: Ego_r.pdb + FableWin.pdb (both name sets) ?PlaceGoods@CAIStateGroup_MoveGoodsBase@@IAE_N...
//   (protected; Ego_r donor order OnActivate < FetchGoods < PlaceGoods < Cleanup = retail 0x00915880 < 0x00915890 < 0x00915B60 < 0x009162F0).
// Shape: `if (!goods.Get()) return false;` before the action-point GetTC (retail head merge: 0x00915BFC `lea ecx,[esp+14h]` hoisted
//   above `je 0x009162B7`); the body after the action-point lookup sits in a block (the address-taken `reached` flag shares retail's
//   1-byte slot [esp+13h] with the first three HasTC bytes); `A` (user still set) and `B` (LookAt result != 2) are explicit
//   free-and-return copies (cross-jumped whole); the PutGoodsDown action is a temporary (destroyed before the result test).
//   The exit structure decides the tail allocation: the PerformAction failure frees the action point INSIDE the pre-route `if`
//   and falls out of the action-point block into the function's final `return false` (shared with the GetTC failure), so c2
//   hoists the by-value vector's `_M_start` load only to the GetTC test (crate_index then takes ebp, 0x00916152) and the vector
//   start splits into {index use, success free} ebx (0x009161E9 / 0x00916254) and the final free (memory, 0x009162C8); the
//   pre-route else is `else if (!goods.Get()) return false; else free;` -- its return copy is one more goods reference, which
//   puts the 8-byte goods ahead of the two equal-density words in c2's slot sort (goods [esp+14h]).
// Leaf: ?PlaceGoods@CAIStateGroup_MoveGoodsBase@@IAE_NV?$CIntelligentPointer@VCThing@@@@MV?$vector@PBVCCharString@@V?$allocator@PBVCCharString@@@std@@@std@@ABVCAIFilterBase@CAIStateGroupBase@@PBVCThing@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MINIONWANDER
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_FILTER
#define FABLE_RETAIL_CAISTATEGROUP_MOVEGOODSBASE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_STATTYPE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PUTGOODSDOWN
#define FABLE_RETAIL_CTCSOLIDTOTHING
#define FABLE_RETAIL_CTCCRATESTACK
#define FABLE_RETAIL_CTCEXCLUSIVEUSE
#define FABLE_RETAIL_CTCUSERACTIONPOINTBASE
#include "engine/C3DVector.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
#include "fable_string.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIBrain.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCCrateStack.h"
#include "engine/CTCExclusiveUse.h"
#include "engine/CTCSolidToThing.h"
#include "engine/CTCUserActionPointBase.h"
#include "engine/CCreatureAction_PutGoodsDown.h"
#include "engine/CAIStateGroup_WorkBase.h"
#include "engine/CAIStateGroup_MoveGoodsBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/GFRandom.h"

bool CAIStateGroup_MoveGoodsBase::PlaceGoods(CIntelligentPointer<CThing> destination, float distance,
    std::vector<const CCharString*> goods_names, const CAIFilterBase& filter, const CThing* route_thing)
{
    if (!destination->HasTC(TCI_CRATE_STACK))
        return false;
    const CTCCarrying* carrying;
    GetCreature().PeekTC(&carrying);
    CIntelligentPointer<CThing> goods(carrying->GetThingInPrimarySlot());
    if (!goods.Get())
        return false;
    CTCSingleUserActionPoints* action_points;
    if (goods->GetTC(&action_points)) {
        // wait for a free action point on the goods
        bool reached;
        long timeout = CWorld::GetFrame() - (long)(CMainGameComponent::GetConstantFPS() * -10.0f);
        C3DVector position;
        CRightHandedSet rh_set;
        long index;
        while (!action_points->GetFreeActionPoint(GetCreature(), position, rh_set, index)) {
            if (AIAction_MinionWander((EMovementType)3, 0) == 1 || CWorld::GetFrame() > timeout || !filter() || !destination.Get() || !goods.Get())
                return false;
        }

        // the stand-off from the destination: its facing direction scaled by the point's 2D distance from the goods
        CTCPhysicsBase* physics;
        destination->GetTC(&physics);
        C3DVector forward = physics->GetRHSet().GetForward();
        float offset_distance = (position - goods->GetPos()).GetMagnitude2D();
        C3DVector offset = forward * offset_distance;
        if (route_thing && (destination->GetPos() - GetCreature().GetPos()).GetSquaredMagnitude() > 400.0f)
            AIAction_MoveToPreCalcRoute(*route_thing, reached, (EMovementType)3, filter, true);

        if (destination.Get() && goods.Get()) {
            EAIActionReturn result;
            do {
                result = AIAction_MoveToPosProximity((EAIActionRepeat)2, destination->GetPos() + offset, 5.0f, reached, (EMovementType)3,
                    true, false, true, 0, true, 0.0f);
                if (result == 1 || result == 3 || !filter() || !destination.Get() || !goods.Get()) {
                    if (goods.Get())
                        action_points->SetActionPointAsFree(index);
                    return false;
                }
            } while (result == 4);

            // wait for the destination to be free, then claim it
            CTCExclusiveUse* exclusive_use = 0;
            if (destination->GetTC(&exclusive_use)) {
                if (exclusive_use->GetUser()) {
                    long end_frame = CWorld::GetFrame() + 50 + GFRandom(50, GetWorld().GetWorldSeed());
                    do {
                        result = AIAction_MinionWander((EMovementType)3, 0);
                    } while (exclusive_use->GetUser() && result != 1 && CWorld::GetFrame() < end_frame && filter() && destination.Get() && goods.Get());
                }
                if (exclusive_use->GetUser()) {
                    action_points->SetActionPointAsFree(index);
                    return false;
                }
                exclusive_use->SetUser(&GetCreature());
            }

            if (!destination.Get() || !goods.Get()) {
                if (goods.Get())
                    action_points->SetActionPointAsFree(index);
                return false;
            }
            do {
                result = AIAction_MoveToPos((EAIActionRepeat)2, destination->GetPos() + offset, reached, (EMovementType)3, true, true, 0, true, 1.0f);
            } while (result == 4 && filter());
            do {
                result = AIAction_LookAtPos((EAIActionRepeat)2, destination->GetPos(), 17, 0.0027777f);
            } while (result == 4 && filter());
            if (result != 2) {
                action_points->SetActionPointAsFree(index);
                return false;
            }
            // put the goods down on the stack
            DroppingGoods = true;
            long crate_index;
            CTCCrateStack* crate_stack;
            if (destination->GetTC(&crate_stack) && crate_stack)
                crate_index = crate_stack->GetNumCrates();
            else
                crate_index = 0;
            CTCSolidToThing* solid = 0;
            if (carrying->GetThingInPrimarySlot()->HasTC(TCI_SOLID_TO_THING))
                carrying->GetThingInPrimarySlot()->GetTC(&solid);
            else
                solid = static_cast<CTCSolidToThing*>(carrying->GetThingInPrimarySlot()->AddTC("CTCSolidToThing", true, 0));
            if (solid)
                solid->AddThingNotSolidTo(&GetCreature());
            result = AIAction_PerformActionAndWait(CCreatureAction_PutGoodsDown(GetCreature(), *goods_names[crate_index]), false);
            if (result == 2) {
                if (exclusive_use)
                    exclusive_use->SetUser(0);
                action_points->SetActionPointAsFree(index);
                return true;
            }
            action_points->SetActionPointAsFree(index);
        } else if (!goods.Get()) {
            return false;
        } else {
            action_points->SetActionPointAsFree(index);
        }
    }
    return false;
}
