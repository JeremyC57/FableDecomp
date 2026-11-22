// CTCTavernTable::GetSeat — reserve a named seat and decode its zero-based index.
// Leaf: ?GetSeat@CTCTavernTable@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ3PBVCCharString@@@Z
// Flags: default
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CCHARSTRING_CONTAINS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS
#define FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS_GETNAME
#define FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS_STATTYPE_PASS27
#define FABLE_RETAIL_CTCTAVERNTABLE_GETSEAT
#define FABLE_RETAIL_CTCTAVERNTABLE_FULL_LAYOUT
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCNamedUserActionPoints.h"
#include "engine/CTCTavernTable.h"

long FABLE_FASTCALL GFCharStringToInt(const CCharString& text);

bool CTCTavernTable::GetSeat(CThing& thing, C3DVector& pos, CRightHandedSet& rhset,
                             long& action_point_index, long& seat_index, const CCharString* text)
{
    if (!AvailableForUse)
        return false;

    CCharString name("ACTION_POINT_SIT_");
    if (text)
        name = *text;

    CTCNamedUserActionPoints* named_user_action_points;
    GetParentThing().GetTC(&named_user_action_points);
    if (!named_user_action_points->GetFreeNamedActionPoint(thing, pos, rhset, name, action_point_index, 0, true))
        return false;

    CCharString find_text;
    named_user_action_points->GetName(action_point_index, find_text);
    if (find_text.Contains(CCharString("SIT")))
        seat_index = GFCharStringToInt(find_text.GetSubString(ActionPointSitStrlen, find_text.GetLength()));
    else if (find_text.Contains(CCharString("GAME")))
        seat_index = GFCharStringToInt(find_text.GetSubString(ActionPointGameMasterStrlen, find_text.GetLength()));
    else if (find_text.Contains(CCharString("HERO")))
        seat_index = GFCharStringToInt(find_text.GetSubString(ActionPointHeroStrlen, find_text.GetLength()));

    if (seat_index <= 0)
        return false;
    --seat_index;
    return true;
}