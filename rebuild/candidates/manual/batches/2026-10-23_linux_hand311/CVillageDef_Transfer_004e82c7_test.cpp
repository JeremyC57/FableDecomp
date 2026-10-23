// Behaviour test for CVillageDef::Transfer (0x004E82C7): every member is transferred once, in retail order.
#include <cstdio>
enum EVillageStanceToHero { VILLAGE_STANCE_DEFAULT };
enum EVillageTaskType { VILLAGE_TASK_DEFAULT };
struct CDefString { long Index; };
struct CCharString { void* PData; };
struct CTaskVector { EVillageTaskType* a; EVillageTaskType* b; EVillageTaskType* c; };
struct CLongVector { long* a; long* b; long* c; };

static const void* g_seen[64];
static int g_kind[64];
static int g_count = 0;
static void Record(const void* p, int kind) { if (g_count < 64) { g_seen[g_count] = p; g_kind[g_count] = kind; } ++g_count; }

struct CPersistContext {
    void Transfer(CTaskVector& v) { Record(&v, 1); }
    void Transfer(CDefString& v) { Record(&v, 2); }
    void Transfer(EVillageStanceToHero& v) { Record(&v, 3); }
    void Transfer(long& v) { Record(&v, 4); }
    void Transfer(CLongVector& v) { Record(&v, 5); }
    void Transfer(bool& v) { Record(&v, 6); }
    void Transfer(CCharString& v) { Record(&v, 7); }
};

struct CVillageDef {
    CTaskVector Tasks; CDefString PatrolTrackName; EVillageStanceToHero DefaultStanceToHero;
    long SecondsSecuritySweepLasts; long WarehouseCrateDef;
    CLongVector StoryStartTimes; long StoryDuration; CLongVector SchoolStartTimes; long SchoolDuration;
    CLongVector WorkStartTimes; long WorkDuration; CLongVector CookStartTimes; long CookDuration;
    CLongVector EatStartTimes; long EatDuration; CLongVector TavernStartTimes; long TavernDuration;
    CLongVector GamingStartTimes; long GamingDuration;
    long SleepAdultStartTime, SleepAdultEndTime, SleepChildStartTime, SleepChildEndTime, SleepElderlyStartTime,
         SleepElderlyEndTime, HouseLightsOnTime, HouseLightsOffTime, SociableStartTime, SociableFinishTime,
         CurfewStartTime, CurfewFinishTime;
    bool EnableAutoPlacement, AllowHouseBuying, HearTownCrier;
    CCharString CurfewStartSound, CurfewEndSound, WorkStartSound, WorkEndSound;
    long BribeCost, FramesBetweenGuardSnoop, FramesBetweenGuardSnoopNightShops, FramesBeforeGuardSnoop,
         FramesBeforeGuardSnoopNightShops;
    void Transfer(CPersistContext& context)
    {
        context.Transfer(Tasks); context.Transfer(PatrolTrackName); context.Transfer(DefaultStanceToHero);
        context.Transfer(SecondsSecuritySweepLasts); context.Transfer(WarehouseCrateDef);
        context.Transfer(SleepAdultStartTime); context.Transfer(SleepAdultEndTime);
        context.Transfer(SleepChildStartTime); context.Transfer(SleepChildEndTime);
        context.Transfer(SleepElderlyStartTime); context.Transfer(SleepElderlyEndTime);
        context.Transfer(HouseLightsOnTime); context.Transfer(HouseLightsOffTime);
        context.Transfer(SociableStartTime); context.Transfer(SociableFinishTime);
        context.Transfer(CurfewStartTime); context.Transfer(CurfewFinishTime);
        context.Transfer(StoryStartTimes); context.Transfer(StoryDuration);
        context.Transfer(SchoolStartTimes); context.Transfer(SchoolDuration);
        context.Transfer(WorkStartTimes); context.Transfer(WorkDuration);
        context.Transfer(CookStartTimes); context.Transfer(CookDuration);
        context.Transfer(EatStartTimes); context.Transfer(EatDuration);
        context.Transfer(TavernStartTimes); context.Transfer(TavernDuration);
        context.Transfer(GamingStartTimes); context.Transfer(GamingDuration);
        context.Transfer(EnableAutoPlacement); context.Transfer(AllowHouseBuying); context.Transfer(HearTownCrier);
        context.Transfer(CurfewStartSound); context.Transfer(CurfewEndSound);
        context.Transfer(WorkStartSound); context.Transfer(WorkEndSound);
        context.Transfer(BribeCost); context.Transfer(FramesBetweenGuardSnoop);
        context.Transfer(FramesBetweenGuardSnoopNightShops); context.Transfer(FramesBeforeGuardSnoop);
        context.Transfer(FramesBeforeGuardSnoopNightShops);
    }
};

int main()
{
    static CVillageDef def;
    CPersistContext context;
    def.Transfer(context);
    int ok = g_count == 43;
    ok = ok && g_seen[0] == &def.Tasks && g_kind[0] == 1;
    ok = ok && g_seen[1] == &def.PatrolTrackName && g_kind[1] == 2;
    ok = ok && g_seen[2] == &def.DefaultStanceToHero && g_kind[2] == 3;
    ok = ok && g_seen[5] == &def.SleepAdultStartTime && g_seen[16] == &def.CurfewFinishTime;
    ok = ok && g_seen[17] == &def.StoryStartTimes && g_kind[17] == 5 && g_seen[30] == &def.GamingDuration;
    ok = ok && g_seen[31] == &def.EnableAutoPlacement && g_kind[33] == 6;
    ok = ok && g_seen[34] == &def.CurfewStartSound && g_kind[37] == 7;
    ok = ok && g_seen[42] == &def.FramesBeforeGuardSnoopNightShops && g_kind[42] == 4;
    if (!ok) { std::printf("004e82c7_TEST FAIL\n"); return 1; }
    std::printf("004e82c7_TEST PASS\n");
    return 0;
}
