// Flags: (none)
// Leaf: ?GetServingPoint@CTCTavernTable@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ3PAPAVCThingObject@@W4ServingMode@1@_N@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS
#define FABLE_RETAIL_CTCUSERACTIONPOINTBASE_SETFREE
#define FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS_GETNAME
#define FABLE_RETAIL_CTCTAVERNTABLE_FULL_LAYOUT
#define FABLE_RETAIL_TAVERN_SERVING_PASS29
#define FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS_STATTYPE_PASS27
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCNamedUserActionPoints.h"
#include "engine/CThingObject.h"
#include "engine/CTCTavernTable.h"
#include "engine/CTCTavernTankard.h"


bool CTCTavernTable::GetServingPoint(CThing& thing, C3DVector& pos, CRightHandedSet& rhset, long& action_point_index, long& seat_index,
                                     CThingObject** p_tankard, ServingMode serving_mode, bool occupied)
{
    if (!AvailableForUse)
        return false;
    long starting_index = 0;
    for (;;) {
        CTCNamedUserActionPoints* named_user_action_points;
        GetParentThing().GetTC(&named_user_action_points);
        long str_len;
        if (occupied) {
            if (!named_user_action_points->GetFullNamedActionPoint(thing, "ACTION_POINT_SIT_", action_point_index, starting_index)) {
                if (!named_user_action_points->GetFullNamedActionPoint(thing, "ACTION_POINT_GAME_MASTER_", action_point_index, starting_index)) {
                    action_point_index = -1;
                    return false;
                }
                str_len = ActionPointGameMasterStrlen;
            } else
                str_len = ActionPointSitStrlen;
        } else {
            if (!named_user_action_points->GetFreeNamedActionPoint(thing, pos, rhset, "ACTION_POINT_SIT_", action_point_index, starting_index, true)) {
                action_point_index = -1;
                return false;
            }
            str_len = ActionPointSitStrlen;
            named_user_action_points->SetActionPointAsFree(action_point_index);
        }
        CCharString name;
        named_user_action_points->GetName(action_point_index, name);
        CCharString number = name.GetSubString(str_len, name.GetLength());
        long val = GFCharStringToInt(number);
        bool needs_attending;
        switch (serving_mode) {
        case SERVE_DRINK: {
            if (val <= 0 || (unsigned long)val > Tankards.size())
                return false;
            if (!Tankards[val - 1].Get())
                return false;
            CTCTavernTankard* tc_tavern_tankard = 0;
            Tankards[val - 1]->GetTC(&tc_tavern_tankard);
            if (!tc_tavern_tankard)
                return false;
            needs_attending = tc_tavern_tankard->Fullness == 0.0f && GetSeatState(val - 1) == occupied;
            break;
        }
        case SERVE_FOOD:
            if (val <= 0 || (unsigned long)val > Food.size())
                return false;
            needs_attending = Food[val - 1].Get() == 0;
            break;
        case REMOVE_FOOD:
            if (val <= 0 || (unsigned long)val > Food.size())
                return false;
            needs_attending = Food[val - 1].Get() != 0;
            break;
        default:
            needs_attending = false;
            break;
        }
        if (needs_attending) {
            if (serving_mode == SERVE_DRINK)
                *p_tankard = Tankards[val - 1].Get();
            CCharString stand_point("ACTION_POINT_MAID_");
            stand_point += number.GetLength() > 1 ? number : "0" + number;
            if (named_user_action_points->GetFreeNamedActionPoint(thing, pos, rhset, stand_point, action_point_index, 0, true)) {
                seat_index = val - 1;
                return true;
            }
        }
        starting_index = action_point_index + 1;
    }
}