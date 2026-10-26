// Behaviour test for 00969760: src/00969760.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CAIStateGroupBase::WaitForMessageEvent(long waitMode, const CFilter_Bind2& filter, CMessageEvent** event)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroupBase::WaitForMessageEvent(long waitMode, const CFilter_Bind2& filter, CMessageEvent** event) -- AI state-group coroutine helper: poll the world's message-event manager for the most recent message passing a Bind2<IsOfType, CanBeSeenOrHeard> filter, yielding a frame between polls
// Leaf: WaitForMessageEvent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00969760, length 120 bytes (ret 0Ch).
// Callees: CAIStateGroupBase::IsStopping @ 0x008FD010, CAIStateGroupBase::GetPWorld @ 0x008FCFA0 (landed names),
// CMessageEventManager::FindMostRecentMessage<CFilter_Bind2> @ 0x008D46D0; virtual slot 2 (+8) of the brain at +4
// (the coroutine yield).
// Label correction: the manifest's "CCOE_AltarEvil::Main" is propagated over byte-identical bodies; this is a
// CAIStateGroupBase template helper instantiated per filter type (0x0091A670 is the twin for the other filter).
// Caller: 0x009683D8.
// Shape: returns 1 when the state group is stopping (checked on entry and after every yield), 2 when a message was
// found (the result is stored through the out pointer before the null test), and 4 once a single poll failed unless
// waitMode is 1 (wait forever: `cmp ebp,1 ; mov eax,4 ; je loop` -- a result local set to 4 at the bottom of the
// do/while body, the 2 and 1 cases `break` out with their own value; returning the constants directly puts the
// `mov eax,4` after the branch). The world's message manager (+0x60) is read into a
// local before the filter is pushed (the chained call pushes the argument first).
class CMessageEvent;
struct CFilter_Bind2;                                  // the bound message filter, passed by address
class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage(const CFilter_Bind2& filter);   // 0x008D46D0
};
class CWorld { public:
    unsigned char         _pad[0x60];
    CMessageEventManager* MessageEventManager; // +0x60
};
struct CBrain {
    virtual void v00();
    virtual void v01();
    virtual void Yield();                     // slot 2 (+8)
};

class CAIStateGroupBase {
public:
    void*   __vftable;                        // +0x00
    CBrain* PBrain;                           // +0x04
    bool IsStopping();                        // 0x008FD010
    CWorld* GetPWorld_008fcfa0();                      // 0x008FCFA0 (PBrain->+0x1c->+0x24)
    long WaitForMessageEvent(long waitMode, const CFilter_Bind2& filter, CMessageEvent** event);
};

long CAIStateGroupBase::WaitForMessageEvent(long waitMode, const CFilter_Bind2& filter, CMessageEvent** event)
{
    if (IsStopping())
        return 1;
    long result;
    do {
        CMessageEventManager* manager = GetPWorld_008fcfa0()->MessageEventManager;
        *event = manager->FindMostRecentMessage(filter);
        if (*event) {
            result = 2;
            break;
        }
        PBrain->Yield();
        if (IsStopping()) {
            result = 1;
            break;
        }
        result = 4;
    } while (waitMode == 1);
    return result;
}

// ---- stubs and driver ----
void CBrain::v00() {}
void CBrain::v01() {}
static char g_log[128]; static int g_logn = 0;
static void Log(char c) { if (g_logn < 127) { g_log[g_logn++] = c; g_log[g_logn] = 0; } }
static const char* g_stopScript = ""; static int g_stopIndex = 0;
bool CAIStateGroupBase::IsStopping() { Log('S'); char c = g_stopScript[g_stopIndex]; if (c) ++g_stopIndex; return c == '1'; }
static CMessageEventManager g_manager;
static CWorld g_world;
CWorld* CAIStateGroupBase::GetPWorld_008fcfa0() { Log('W'); return &g_world; }
static CMessageEvent* g_found[8]; static int g_findIndex = 0; static const CFilter_Bind2* g_filterSeen; static CMessageEventManager* g_findThis;
CMessageEvent* CMessageEventManager::FindMostRecentMessage(const CFilter_Bind2& filter)
{ Log('F'); g_filterSeen = &filter; g_findThis = this; return g_found[g_findIndex++ & 7]; }
struct CTestBrain : CBrain { int yields; virtual void Yield() { ++yields; Log('Y'); } };
void CBrain::Yield() { }
static int g_filterStore;
static const CFilter_Bind2& FILTER = *(const CFilter_Bind2*)&g_filterStore;
static CMessageEvent* const EVENT = (CMessageEvent*)0x4444;
static void Reset(const char* stops) { g_stopScript = stops; g_stopIndex = 0; g_findIndex = 0; g_logn = 0; g_log[0] = 0; memset(g_found, 0, sizeof g_found); }
int main()
{
    g_world.MessageEventManager = &g_manager;
    CTestBrain brain; brain.yields = 0;
    CAIStateGroupBase group; group.__vftable = 0; group.PBrain = &brain;
    CMessageEvent* event = (CMessageEvent*)1;
    Reset("1");                                   // stopping on entry
    CHECK(group.WaitForMessageEvent(2, FILTER, &event) == 1); CHECK(event == (CMessageEvent*)1); CHECK(strcmp(g_log, "S") == 0);
    Reset("0"); g_found[0] = EVENT;               // found at once
    CHECK(group.WaitForMessageEvent(2, FILTER, &event) == 2); CHECK(event == EVENT); CHECK(g_filterSeen == &FILTER); CHECK(g_findThis == &g_manager);
    CHECK(strcmp(g_log, "SWF") == 0);
    Reset("00");                                  // one poll fails, mode 2 times out
    CHECK(group.WaitForMessageEvent(2, FILTER, &event) == 4); CHECK(event == 0); CHECK(strcmp(g_log, "SWFYS") == 0);
    Reset("01");                                  // stopped while waiting
    CHECK(group.WaitForMessageEvent(1, FILTER, &event) == 1); CHECK(strcmp(g_log, "SWFYS") == 0);
    Reset("0000"); g_found[3] = EVENT;            // mode 1 waits until a message turns up
    CHECK(group.WaitForMessageEvent(1, FILTER, &event) == 2); CHECK(event == EVENT); CHECK(strcmp(g_log, "SWFYSWFYSWFYSWF") == 0);
    CHECK(brain.yields == 1 + 1 + 3);
    if (fails == 0) printf("00969760_TEST PASS\n"); else printf("00969760_TEST FAIL\n");
    return fails;
}