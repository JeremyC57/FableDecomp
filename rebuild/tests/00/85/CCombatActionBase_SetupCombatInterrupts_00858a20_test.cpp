// Behaviour test for 0x00858A20 CCombatActionBase::SetupCombatInterrupts: runs the reconstructed body over a stub animation
// event manager; checks every event-driven member (interrupt time scaled by the creature's stats speed, reset-combo time,
// movement-interrupt frame plus the anim delay, speed-variation start/end and the -1 fallback), the stats def release and the
// fetched event list being freed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

long g_FramesPerSecond = 15;

static int g_strings = 0;
class CCharString { public:
    char m_text[64];
    CCharString(const char* text, long = -1) { std::strcpy(m_text, text); ++g_strings; }
    CCharString(const CCharString& o) { std::strcpy(m_text, o.m_text); ++g_strings; }
    ~CCharString() { --g_strings; }
};
CCharString g_combtActionFinishedEventName("COMBT_ACTION_FINISHED");
CCharString g_strikeEventResetComboEventName("STRIKE_EVENT_RESET_COMBO");
CCharString g_allowMovementInterruptEventName("ALLOW_MOVEMENT_INTERRUPT");

struct CAnimEvent {
    CCharString Name;
    float       Time;
};
struct CAnimEventVector {
    CAnimEvent* First;
    CAnimEvent* Last;
    CAnimEvent* End;
    CAnimEventVector() : First(0), Last(0), End(0) { }
    static void Destroy(CAnimEvent* first, CAnimEvent* last)
    {
        for (; first != last; ++first)
            first->~CAnimEvent();
    }
    ~CAnimEventVector()
    {
        Destroy(First, Last);
        if (First)
            free(First);
    }
};

struct SEventEntry { const char* name; float time; };
static SEventEntry g_events[8];
static int g_eventCount = 0;
static unsigned long g_lastAnimID = 0;
static int g_getEventsCalls = 0;
namespace NAnimationEvents {
class CManager { public:
    virtual ~CManager() { }
    virtual void GetEvents(unsigned long animID, CAnimEventVector& events) const
    {
        ++g_getEventsCalls; g_lastAnimID = animID;
        events.First = (CAnimEvent*)std::malloc(sizeof(CAnimEvent) * 2);
        for (int i = 0; i < 2; ++i) {
            CAnimEvent* e = &events.First[i];
            new (&e->Name) CCharString(i == 0 ? "A" : "B");
            e->Time = (float)i;
        }
        events.Last = events.End = events.First + 2;
    }
    bool AnimHasEvent(unsigned long animID, const CCharString& eventName) const
    {
        if (animID != 42) return false;
        for (int i = 0; i < g_eventCount; ++i)
            if (std::strcmp(g_events[i].name, eventName.m_text) == 0) return true;
        return false;
    }
    float GetEventTimeInSeconds(unsigned long animID, const CCharString& eventName) const
    {
        if (animID != 42) return -100.0f;
        for (int i = 0; i < g_eventCount; ++i)
            if (std::strcmp(g_events[i].name, eventName.m_text) == 0) return g_events[i].time;
        return -50.0f;
    }
};
}
static NAnimationEvents::CManager g_manager;
NAnimationEvents::CManager* GetAnimManager() { return &g_manager; }

static int g_destroyed = 0;
struct CDefBase {
    virtual void v00() { }
    virtual void Destroy() { ++g_destroyed; }
    long RefCount;
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CCreatureStatsDef : public CDefBase { public:
    float Speed;
};
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p) m_p->Release(); }
    T* operator->() const { return m_p; }
};
static CCreatureStatsDef* g_stats = 0;
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CCreatureStatsDef>& out)
    {
        if (!g_stats) return false;
        ++g_stats->RefCount; out.m_p = g_stats; return true;
    }
};
class CThing { public:
    CDefClassBase* PDef;
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
class CAnimationEntry { public:
    unsigned long AnimID;
};

class CCombatActionBase { public:
    long m_delay;
    virtual ~CCombatActionBase() { }
    virtual long GetAnimDelayTime() const { return m_delay; }
    CBaseIntelligentPointer PCreature;
    long                    TotalTime;
    CAnimationEntry*        PAnimEntry;
    float StrikeEventInterruptTime;
    float StrikeEventResetComboTime;
    long  MovementEventInterruptFrame;
    float StrikeSpeedVariationStartTime;
    float StrikeSpeedVariationEndTime;
    bool  HasStrikeEventInterrupt;
    bool  HasStrikeEventResetCombo;
    bool  HasMovementEventInterrupt;
    void SetupCombatInterrupts();
};

void CCombatActionBase::SetupCombatInterrupts()
{
    float framesPerSecond = (float)g_FramesPerSecond;
    float timeScale = 1.0f;
    unsigned long animID = PAnimEntry->AnimID;
    NAnimationEvents::CManager* manager = GetAnimManager();
    CAnimEventVector events;
    manager->GetEvents(animID, events);
    if (manager->AnimHasEvent(animID, g_combtActionFinishedEventName)) {
        HasStrikeEventInterrupt = true;
        float eventTime = manager->GetEventTimeInSeconds(animID, g_combtActionFinishedEventName);
        float totalTime = (TotalTime - GetAnimDelayTime()) / framesPerSecond;
        float speed = 1.0f;
        CDefPointer<CCreatureStatsDef> stats;
        CDefClassBase* creatureDef = PCreature.GetPItem()->PDef;
        if (creatureDef->GetSubDef(stats))
            speed = stats->Speed;
        StrikeEventInterruptTime = eventTime + (totalTime - eventTime) * (1.0f - speed) * 0.5f;
    }
    if (manager->AnimHasEvent(animID, g_strikeEventResetComboEventName)) {
        HasStrikeEventResetCombo = true;
        StrikeEventResetComboTime = manager->GetEventTimeInSeconds(animID, g_strikeEventResetComboEventName);
    }
    if (manager->AnimHasEvent(animID, g_allowMovementInterruptEventName)) {
        HasMovementEventInterrupt = true;
        MovementEventInterruptFrame = (long)(manager->GetEventTimeInSeconds(animID, g_allowMovementInterruptEventName) * framesPerSecond);
    }
    bool hasStart;
    {
        CCharString name("STRIKE_SPEED_VARIATION_START");
        hasStart = manager->AnimHasEvent(animID, name);
    }
    if (hasStart) {
        {
            CCharString name("STRIKE_SPEED_VARIATION_START");
            StrikeSpeedVariationStartTime = manager->GetEventTimeInSeconds(animID, name);
        }
        bool hasEnd;
        {
            CCharString name("STRIKE_SPEED_VARIATION_END");
            hasEnd = manager->AnimHasEvent(animID, name);
        }
        if (hasEnd) {
            CCharString name("STRIKE_SPEED_VARIATION_END");
            StrikeSpeedVariationEndTime = manager->GetEventTimeInSeconds(animID, name);
        } else {
            StrikeSpeedVariationStartTime = -1.0f;
            StrikeSpeedVariationEndTime = -1.0f;
        }
    }
    StrikeEventInterruptTime *= timeScale;
    StrikeEventResetComboTime *= timeScale;
    StrikeSpeedVariationStartTime *= timeScale;
    StrikeSpeedVariationEndTime *= timeScale;
    if (HasMovementEventInterrupt) {
        MovementEventInterruptFrame = (long)(MovementEventInterruptFrame * timeScale);
        MovementEventInterruptFrame += GetAnimDelayTime();
    }
}

static void SetEvent(int i, const char* name, float time) { g_events[i].name = name; g_events[i].time = time; }

static void Reset(CCombatActionBase& a)
{
    a.StrikeEventInterruptTime = 9.0f; a.StrikeEventResetComboTime = 9.0f; a.MovementEventInterruptFrame = 99;
    a.StrikeSpeedVariationStartTime = 9.0f; a.StrikeSpeedVariationEndTime = 9.0f;
    a.HasStrikeEventInterrupt = false; a.HasStrikeEventResetCombo = false; a.HasMovementEventInterrupt = false;
}

int main()
{
    bool ok = true;
    int baseStrings = g_strings;
    CDefClassBase creatureDef; CThing creature; creature.PDef = &creatureDef;
    CAnimationEntry entry; entry.AnimID = 42;
    CCreatureStatsDef stats; stats.RefCount = 1; stats.Speed = 0.5f;

    CCombatActionBase action;
    action.m_delay = 10; action.PCreature.m_pItem = &creature; action.TotalTime = 40; action.PAnimEntry = &entry;

    // every event, stats speed 0.5: interrupt = 0.5 + (2.0 - 0.5) * 0.5 * 0.5 = 0.875
    g_eventCount = 5;
    SetEvent(0, "COMBT_ACTION_FINISHED", 0.5f);
    SetEvent(1, "STRIKE_EVENT_RESET_COMBO", 1.25f);
    SetEvent(2, "ALLOW_MOVEMENT_INTERRUPT", 0.5f);
    SetEvent(3, "STRIKE_SPEED_VARIATION_START", 0.25f);
    SetEvent(4, "STRIKE_SPEED_VARIATION_END", 0.75f);
    g_stats = &stats;
    Reset(action);
    action.SetupCombatInterrupts();
    ok = ok && g_getEventsCalls == 1 && g_lastAnimID == 42 && g_strings == baseStrings;
    ok = ok && action.HasStrikeEventInterrupt && action.StrikeEventInterruptTime == 0.875f;
    ok = ok && action.HasStrikeEventResetCombo && action.StrikeEventResetComboTime == 1.25f;
    ok = ok && action.HasMovementEventInterrupt && action.MovementEventInterruptFrame == 7 + 10;
    ok = ok && action.StrikeSpeedVariationStartTime == 0.25f && action.StrikeSpeedVariationEndTime == 0.75f;
    ok = ok && stats.RefCount == 1 && g_destroyed == 0;

    // no stats def: speed 1.0 leaves the event time; start without end: both -1
    g_stats = 0;
    g_eventCount = 4;
    Reset(action);
    action.SetupCombatInterrupts();
    ok = ok && action.StrikeEventInterruptTime == 0.5f && action.StrikeSpeedVariationStartTime == -1.0f;
    ok = ok && action.StrikeSpeedVariationEndTime == -1.0f && g_strings == baseStrings;

    // only reset combo: others untouched, movement frame not re-based
    g_eventCount = 1;
    SetEvent(0, "STRIKE_EVENT_RESET_COMBO", 2.0f);
    Reset(action);
    action.SetupCombatInterrupts();
    ok = ok && !action.HasStrikeEventInterrupt && action.StrikeEventInterruptTime == 9.0f;
    ok = ok && action.HasStrikeEventResetCombo && action.StrikeEventResetComboTime == 2.0f;
    ok = ok && !action.HasMovementEventInterrupt && action.MovementEventInterruptFrame == 99;
    ok = ok && action.StrikeSpeedVariationStartTime == 9.0f && action.StrikeSpeedVariationEndTime == 9.0f;

    // a Has flag already set keeps re-basing the old frame
    g_eventCount = 0;
    Reset(action);
    action.HasMovementEventInterrupt = true; action.MovementEventInterruptFrame = 5;
    action.SetupCombatInterrupts();
    ok = ok && action.MovementEventInterruptFrame == 15;

    // the last stats reference is destroyed by the local def pointer
    g_eventCount = 1;
    SetEvent(0, "COMBT_ACTION_FINISHED", 0.5f);
    g_stats = &stats; stats.RefCount = 0;
    Reset(action);
    action.SetupCombatInterrupts();
    ok = ok && g_destroyed == 1 && g_strings == baseStrings && g_getEventsCalls == 5;

    if (!ok) { std::printf("00858a20_TEST FAIL\n"); return 1; }
    std::printf("00858a20_TEST PASS\n");
    return 0;
}