// Behaviour test for 00732950: final.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CTCClock::FrameUpdate()
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifdef _MSC_VER
#define snprintf _snprintf
#else
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __int64 long long
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCClock::FrameUpdate() -- advance the clock hands' animation by the time-of-day change since the last update and remember the current time of day
//
// Retail 0x00732950, 133 bytes (plain `ret`). Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020, CTCAnimationComplex::GetAnimation @ 0x0070B460 (manifest "CWorld::DrawGetEnvironment",
// propagated), CTCClock::GetHandAnimationUpdateAmount @ 0x00732730 (landed), CTCBase::GetWorld
// @ 0x00686A70 (manifest "CViewBase::GetDisplayEngine", propagated), CAnimateThing::UpdateFrameLogic
// @ 0x0070E8B0.
// Layout: Ego_r.pdb CTCClock (0x14): PDef +0x0c, LastTimeUpdated +0x10; the world's +0x1c record holds
// the normalised time of day at +8 (landed GetHandAnimationUpdateAmount 0x00732730).
// Shape: the animation-complex lookup is not tested, so the absent arm calls GetAnimation through the
// never-written local (it reads the stale key frame word, `mov ecx,[esp+8]`); the call is spelled in
// both arms (hit arm on the lookup expression) so VC7.1 cross-jumps them with the component already in
// ecx -- one call after a joined pointer copies it through eax (+2 bytes). The animation is a named
// local (esi); the 12-byte update
// record {amount, 0, 0} is built from the float result (`fstp`) and the time of day is copied as a
// dword into LastTimeUpdated just before the update call.
class CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};

struct CAnimFrameUpdate {                                   // 12-byte update record
    float Time;                                             // +0x00
    float Reserved0;                                        // +0x04
    float Reserved1;                                        // +0x08
    CAnimFrameUpdate(float time) : Time(time), Reserved0(0.0f), Reserved1(0.0f) {}
};

class CAnimateThing { public:
    void UpdateFrameLogic(const CAnimFrameUpdate& update);  // 0x0070E8B0
};

class CTCAnimationComplex { public:
    CAnimateThing* GetAnimation();                          // 0x0070B460
};

struct CTimeOfDay {
    unsigned char _pad[8];
    float         Time;                                     // +0x08 normalised time of day
};

class CWorld { public:
    unsigned char _pad[0x1c];
    CTimeOfDay*   TimeOfDay;                                // +0x1c
};

class CTCClock { public:
    virtual void v00();
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    unsigned char _pad08[4];
    void*         PDef;                                     // +0x0c CDefPointer<const CClockDef>
    float         LastTimeUpdated;                          // +0x10
    CWorld* GetWorld();                                     // 0x00686A70 (CTCBase)
    float GetHandAnimationUpdateAmount();                   // 0x00732730
    void FrameUpdate();
};

void CTCClock::FrameUpdate()
{
    CTCAnimationComplex* complex;
    CAnimateThing* animation;
    if (ParentThing->HasInterface(0x5a))
        animation = ((CTCAnimationComplex*)ParentThing->GetInterface(0x5a))->GetAnimation();
    else
        animation = complex->GetAnimation();
    CAnimFrameUpdate update(GetHandAnimationUpdateAmount());
    LastTimeUpdated = GetWorld()->TimeOfDay->Time;
    animation->UpdateFrameLogic(update);
}

// ---- stubs ----
static char g_log[2048];
static void logf(const char* fmt, long a = 0, long b = 0, long c = 0) { char t[256]; snprintf(t, sizeof t, fmt, a, b, c); strcat(g_log, t); strcat(g_log, ";"); }
void CTCClock::v00() { }
static CTCAnimationComplex g_complex; static CAnimateThing g_animation; static CWorld g_world; static CTimeOfDay g_timeOfDay;
static float g_amount;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
// the miss path calls through a never-written local: the stub does not touch `this`, so that path can be driven
CAnimateThing* CTCAnimationComplex::GetAnimation() { logf("GetAnimation(%ld)", (long)(this == &g_complex)); return &g_animation; }
CWorld* CTCClock::GetWorld() { logf("GetWorld"); return &g_world; }
float CTCClock::GetHandAnimationUpdateAmount() { logf("GetHandAnimationUpdateAmount"); return g_amount; }
void CAnimateThing::UpdateFrameLogic(const CAnimFrameUpdate& update)
{
    logf("UpdateFrameLogic(%ld,%ld,%ld)", (long)(update.Time * 1000.0f), (long)(update.Reserved0 * 1000.0f + update.Reserved1 * 1000.0f), (long)(this == &g_animation));
}

int main()
{
    static unsigned char thingBytes[0x100]; memset(thingBytes, 0, sizeof thingBytes);
    CThing* thing = (CThing*)thingBytes;
    static CInterfaceMap::SEntry entries[4] = { {3, (CTCBase*)0x11}, {0x5a, (CTCBase*)&g_complex}, {0x70, (CTCBase*)0x22}, {0, (CTCBase*)0} };
    thing->m_interfaces.first = entries; thing->m_interfaces.last = entries + 3;
    memset(&g_world, 0, sizeof g_world); g_world.TimeOfDay = &g_timeOfDay; memset(&g_timeOfDay, 0, sizeof g_timeOfDay);
    CTCClock* clock = (CTCClock*)calloc(1, sizeof(CTCClock));
    new (clock) CTCClock();
    clock->ParentThing = thing;

    // 1. animation complex present: its animation gets a {amount, 0, 0} update and the time of day is latched
    thing->m_presentBits[2] = 1UL << 26; g_amount = 0.25f; g_timeOfDay.Time = 0.5f; clock->LastTimeUpdated = 0.0f; g_log[0] = 0;
    clock->FrameUpdate();
    CHECK(strcmp(g_log, "GetAnimation(1);GetHandAnimationUpdateAmount;GetWorld;UpdateFrameLogic(250,0,1);") == 0);
    CHECK(clock->LastTimeUpdated == 0.5f);

    // 2. a second frame with other values
    g_amount = -1.5f; g_timeOfDay.Time = 0.75f; g_log[0] = 0;
    clock->FrameUpdate();
    CHECK(strcmp(g_log, "GetAnimation(1);GetHandAnimationUpdateAmount;GetWorld;UpdateFrameLogic(-1500,0,1);") == 0);
    CHECK(clock->LastTimeUpdated == 0.75f);

    // 3. the lookup is a lower bound: the 0x5a bit set with no 0x5a entry selects the end slot
    static CInterfaceMap::SEntry entries2[3] = { {3, (CTCBase*)0x11}, {0x70, (CTCBase*)0x22}, {0, (CTCBase*)&g_complex} };
    thing->m_interfaces.first = entries2; thing->m_interfaces.last = entries2 + 2; g_log[0] = 0;
    clock->FrameUpdate();
    CHECK(strcmp(g_log, "GetAnimation(1);GetHandAnimationUpdateAmount;GetWorld;UpdateFrameLogic(-1500,0,1);") == 0);

    // 4. no animation complex: retail still calls GetAnimation (through the never-written local; the stub ignores `this`)
    thing->m_presentBits[2] = 0; g_log[0] = 0;
    clock->FrameUpdate();
    CHECK(strncmp(g_log, "GetAnimation(", 13) == 0);
    CHECK(strstr(g_log, ");GetHandAnimationUpdateAmount;GetWorld;UpdateFrameLogic(-1500,0,1);") != 0);

    if (fails == 0) printf("00732950_TEST PASS\n"); else printf("00732950_TEST FAIL\n");
    return fails != 0;
}