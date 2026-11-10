// CMessageEventManager::FindMostRecentMessage(const CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNot>, CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange> >& filter) const -- the newest queued message of the filter's type whose originator is an AI creature other than the given thing and within the 2D range, or null
// Grade: DIFFER(208v208) -- (agent1019 regrade, unchanged) only the fail-path join reload order: ours reloads this ([esp+14h], edi) before the iterator ([esp+10h], esi), retail the iterator first. c2prio --trace: the split pass over ids [2,3,5] gives this the recycled piece #1 (freed by the const 0x1f split) and the iterator #13/#16, i.e. a numbering effect (GOTCHAS: rough-area reload order). 12 free probes inert: flag-test operand order, literal mask, while / result-local / reference / continue loop bodies, a filter alias, four ThingSearchFilter spellings (two stop the inline expansion, 59 bytes).
// Leaf: ??$FindMostRecentMessage@V?$CFilter_Bind3@VCMessageEventFilter_IsOfType@@V?$CMessageEventFilter_ThingSearchFilter@VCThingFilter_IsNot@@@@V?$CMessageEventFilter_ThingSearchFilter@VCThingFilter_In2DRange@@@@@@@CMessageEventManager@@QBEPBVCMessageEvent@@ABV?$CFilter_Bind3@VCMessageEventFilter_IsOfType@@V?$CMessageEventFilter_ThingSearchFilter@VCThingFilter_IsNot@@@@V?$CMessageEventFilter_ThingSearchFilter@VCThingFilter_In2DRange@@@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00934E50, 206 bytes, thiscall, `ret 4`.
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
