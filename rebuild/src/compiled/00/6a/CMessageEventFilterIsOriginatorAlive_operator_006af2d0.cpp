// CMessageEventFilter_IsOriginatorAlive::operator()(const CMessageEvent& test_event) const -- true when the event's originator exists and is not dead
// Leaf: ??RCMessageEventFilter_IsOriginatorAlive@@QBE_NABVCMessageEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AF2D0, 66 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb ??RCMessageEventFilter_IsOriginatorAlive@@QBE_NABVCMessageEvent@@@Z (message_event_filters.obj #12;
//   Ego_r 62 bytes, same shape). FableWin 0x02111F90: IS_THING_ALIVE<CThing*>(test_event.GetPOriginator<CThing>()).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENT_ORIGINATOR_ORIGINAL
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_YELLFORHELP_UPDATE
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CThing.h"
#include "engine/EMessageEventType.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventFilters.h"
#include "engine/ThingAlive.h"

bool CMessageEventFilter_IsOriginatorAlive::operator()(const CMessageEvent& test_event) const
{
    return IS_THING_ALIVE(test_event.GetPOriginator<CThing>());
}