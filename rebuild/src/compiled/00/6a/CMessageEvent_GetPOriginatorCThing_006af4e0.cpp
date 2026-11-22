// CMessageEvent::GetPOriginator<CThing>() const -- returns the message originator when its intelligent pointer resolves.
// Leaf: ??$GetPOriginator@VCThing@@@CMessageEvent@@QBEPAVCThing@@XZ
// Flags: default
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

template CThing* CMessageEvent::GetPOriginator<CThing>() const;