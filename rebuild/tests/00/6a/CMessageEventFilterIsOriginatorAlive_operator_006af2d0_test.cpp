// Behaviour test for 0x006AF2D0 CMessageEventFilter_IsOriginatorAlive::operator(): the row's defines, includes and body verbatim.
// The originator's intelligent pointer (GetPItem, defined here over the event buffer) yields no thing, a dead thing or a live one;
// only the live one passes. GetPItem is asked only of the event's POriginator (+0x48), never of PTarget.
#include <stdio.h>
#include <string.h>
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

static CBaseObject* g_item = 0;
static const CBaseIntelligentPointer* g_expected = 0;
static int g_calls = 0, g_wrong = 0;
CBaseObject* CBaseIntelligentPointer::GetPItem() const { ++g_calls; if (this != g_expected) ++g_wrong; return g_item; }

static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static double g_eventbuf[sizeof(CMessageEvent) / 8 + 1];
static double g_thingbuf[sizeof(CThing) / 8 + 1];
int main()
{
    memset(g_eventbuf, 0, sizeof(g_eventbuf)); memset(g_thingbuf, 0, sizeof(g_thingbuf));
    const CMessageEvent& event = *reinterpret_cast<const CMessageEvent*>(g_eventbuf);
    CThing* thing = reinterpret_cast<CThing*>(g_thingbuf);
    g_expected = &event.POriginator;
    CMessageEventFilter_IsOriginatorAlive filter;
    g_item = 0;
    check(!filter(event), "no originator -> false");
    g_item = reinterpret_cast<CBaseObject*>(thing);
    reinterpret_cast<unsigned char*>(thing)[0x91] = 1;   // CThing::Dead, bit 0 of +0x91 (protected)
    check(!filter(event), "dead originator -> false");
    reinterpret_cast<unsigned char*>(thing)[0x91] = 0xFE; // every other bit of the byte set: only bit 0 is Dead
    g_calls = 0;
    check(filter(event), "live originator -> true");
    check(g_calls >= 2, "pointer read again for the result");
    check(g_wrong == 0, "only POriginator asked");
    if (!g_fail) printf("006af2d0_TEST PASS\n");
    return g_fail;
}