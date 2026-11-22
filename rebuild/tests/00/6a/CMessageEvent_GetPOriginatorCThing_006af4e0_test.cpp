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

#include <stdio.h>

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseObject::CBaseObject() : IntelligentPointer(0) {}
CBaseObject::~CBaseObject() {}
void CBaseObject::Validate() const {}
void CClass::Save(CCharString&) {}
CCharString::CCharString() : PStringData(0) {}
CCharString::~CCharString() {}

static unsigned int GetPItemCalls;
static CBaseObject* OriginatorItem;

CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CBaseObject* CBaseIntelligentPointer::GetPItem() const
{
    ++GetPItemCalls;
    return OriginatorItem;
}

class TestThing : public CThing {
public:
    void SetTypeFlags(unsigned long flags) { ThingTypeFlags = flags; }
};

int main()
{
    CMessageEvent event;
    TestThing thing;
    OriginatorItem = 0;
    GetPItemCalls = 0;
    if (event.GetPOriginator<CThing>() != 0 || GetPItemCalls != 1)
        return 1;

    OriginatorItem = &thing;
    GetPItemCalls = 0;
    thing.SetTypeFlags(0);
    if (event.GetPOriginator<CThing>() != &thing || GetPItemCalls != 3)
        return 2;

    GetPItemCalls = 0;
    thing.SetTypeFlags(0xffffffffUL);
    if (event.GetPOriginator<CThing>() != &thing || GetPItemCalls != 3)
        return 3;

    puts("006af4e0_TEST PASS");
    return 0;
}