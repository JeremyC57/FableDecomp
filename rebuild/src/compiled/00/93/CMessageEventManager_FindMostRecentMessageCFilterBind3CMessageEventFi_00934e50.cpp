// CMessageEventManager::FindMostRecentMessage(const CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNot>, CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange> >& filter) const -- the newest queued message of the filter's type whose originator is an AI creature other than the given thing and within the 2D range, or null
// Inline-budget: /d2inlT107 expands the original filter guard chain; T106 retains its call.
// Leaf: ??$FindMostRecentMessage@V?$CFilter_Bind3@VCMessageEventFilter_IsOfType@@V?$CMessageEventFilter_ThingSearchFilter@VCThingFilter_IsNot@@@@V?$CMessageEventFilter_ThingSearchFilter@VCThingFilter_In2DRange@@@@@@@CMessageEventManager@@QBEPBVCMessageEvent@@ABV?$CFilter_Bind3@VCMessageEventFilter_IsOfType@@V?$CMessageEventFilter_ThingSearchFilter@VCThingFilter_IsNot@@@@V?$CMessageEventFilter_ThingSearchFilter@VCThingFilter_In2DRange@@@@@@@Z
// Flags: /d2inlT107
// Retail: Fable.exe (TLC, VC7.1) @ 0x00934E50, 208 bytes, thiscall, `ret 4`.
#define FABLE_RETAIL_CMESSAGEEVENT_ORIGINATOR_ORIGINAL
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ORIGINAL_GUARD
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_ORIGINAL_LOCAL
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventManager.h"

typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNot>,
                      CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange> > TNotThingInRangeOfTypeFilter;
template const CMessageEvent* CMessageEventManager::FindMostRecentMessage<TNotThingInRangeOfTypeFilter>(const TNotThingInRangeOfTypeFilter&) const;