// CVillageDef::Transfer(CPersistContext& context) -- persist the village definition: task list, patrol track, stance, the sleep/lights/sociable/curfew clock times, the per-activity start-time vectors and durations, the village flags, the curfew/work sounds and the guard-snoop timings
// Leaf: ?Transfer@CVillageDef
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E82C7, length 579 bytes (virtual, `ret 4`; unaligned entry, def module).
// Callees (typed CPersistContext transfers; the catalog labels on these shared bodies are propagated and wrong):
//   vector<EVillageTaskType> 0x004E85AE, CDefString 0x0045228F, EVillageStanceToHero 0x004E850A, long 0x00431102,
//   vector<long> 0x00466A47, bool 0x0043314A, CCharString 0x00431143.
// Layout: Ego_r.pdb CVillageDef (300 bytes) re-based on the retail bytes: the def base is 0x28 bytes and each STLport
//   vector 12 bytes (16 in the PDB), so PatrolTrackName sits at +0x34 and the sleep times start at +0xb4. No generated
//   engine header for the class.
// Pragma: #pragma optimize("s", on) -- a size TU (as the landed def Transfer bodies): every member address goes through eax.
// Shape: one transfer per member, but the twelve single clock times (SleepAdultStartTime .. CurfewFinishTime) are
//   transferred straight after WarehouseCrateDef, ahead of the activity start-time vector/duration pairs.
enum EVillageStanceToHero { VILLAGE_STANCE_DEFAULT };
enum EVillageTaskType { VILLAGE_TASK_DEFAULT };
class CDefString { public: long Index; };
class CCharString { public: void* PData; };
struct CTaskVector { EVillageTaskType* _M_start; EVillageTaskType* _M_finish; EVillageTaskType* _M_end_of_storage; };
struct CLongVector { long* _M_start; long* _M_finish; long* _M_end_of_storage; };

class CPersistContext { public:
    void Transfer(CTaskVector& value);            // 0x004E85AE
    template <class T> void Transfer(T& value);             // 0x0045228F
    void Transfer(EVillageStanceToHero& value);   // 0x004E850A
    void Transfer(CLongVector& value);            // 0x00466A47
};

class CDefClassBase { public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _body[0x28 - 4];
};

class CVillageDef : public CDefClassBase { public:
    CTaskVector Tasks;                            // +0x28
    CDefString PatrolTrackName;                   // +0x34
    EVillageStanceToHero DefaultStanceToHero;     // +0x38
    long SecondsSecuritySweepLasts;               // +0x3c
    long WarehouseCrateDef;                       // +0x40
    CLongVector StoryStartTimes;                  // +0x44
    long StoryDuration;                           // +0x50
    CLongVector SchoolStartTimes;                 // +0x54
    long SchoolDuration;                          // +0x60
    CLongVector WorkStartTimes;                   // +0x64
    long WorkDuration;                            // +0x70
    CLongVector CookStartTimes;                   // +0x74
    long CookDuration;                            // +0x80
    CLongVector EatStartTimes;                    // +0x84
    long EatDuration;                             // +0x90
    CLongVector TavernStartTimes;                 // +0x94
    long TavernDuration;                          // +0xa0
    CLongVector GamingStartTimes;                 // +0xa4
    long GamingDuration;                          // +0xb0
    long SleepAdultStartTime;                     // +0xb4
    long SleepAdultEndTime;                       // +0xb8
    long SleepChildStartTime;                     // +0xbc
    long SleepChildEndTime;                       // +0xc0
    long SleepElderlyStartTime;                   // +0xc4
    long SleepElderlyEndTime;                     // +0xc8
    long HouseLightsOnTime;                       // +0xcc
    long HouseLightsOffTime;                      // +0xd0
    long SociableStartTime;                       // +0xd4
    long SociableFinishTime;                      // +0xd8
    long CurfewStartTime;                         // +0xdc
    long CurfewFinishTime;                        // +0xe0
    bool EnableAutoPlacement;                     // +0xe4
    bool AllowHouseBuying;                        // +0xe5
    bool HearTownCrier;                           // +0xe6
    CCharString CurfewStartSound;                 // +0xe8
    CCharString CurfewEndSound;                   // +0xec
    CCharString WorkStartSound;                   // +0xf0
    CCharString WorkEndSound;                     // +0xf4
    long BribeCost;                               // +0xf8
    long FramesBetweenGuardSnoop;                 // +0xfc
    long FramesBetweenGuardSnoopNightShops;       // +0x100
    long FramesBeforeGuardSnoop;                  // +0x104
    long FramesBeforeGuardSnoopNightShops;        // +0x108
    virtual void Transfer(CPersistContext& context);
};

void CVillageDef::Transfer(CPersistContext& context)
{
    context.Transfer(Tasks);
    context.Transfer(PatrolTrackName);
    context.Transfer(DefaultStanceToHero);
    context.Transfer(SecondsSecuritySweepLasts);
    context.Transfer(WarehouseCrateDef);
    context.Transfer(SleepAdultStartTime);
    context.Transfer(SleepAdultEndTime);
    context.Transfer(SleepChildStartTime);
    context.Transfer(SleepChildEndTime);
    context.Transfer(SleepElderlyStartTime);
    context.Transfer(SleepElderlyEndTime);
    context.Transfer(HouseLightsOnTime);
    context.Transfer(HouseLightsOffTime);
    context.Transfer(SociableStartTime);
    context.Transfer(SociableFinishTime);
    context.Transfer(CurfewStartTime);
    context.Transfer(CurfewFinishTime);
    context.Transfer(StoryStartTimes);
    context.Transfer(StoryDuration);
    context.Transfer(SchoolStartTimes);
    context.Transfer(SchoolDuration);
    context.Transfer(WorkStartTimes);
    context.Transfer(WorkDuration);
    context.Transfer(CookStartTimes);
    context.Transfer(CookDuration);
    context.Transfer(EatStartTimes);
    context.Transfer(EatDuration);
    context.Transfer(TavernStartTimes);
    context.Transfer(TavernDuration);
    context.Transfer(GamingStartTimes);
    context.Transfer(GamingDuration);
    context.Transfer(EnableAutoPlacement);
    context.Transfer(AllowHouseBuying);
    context.Transfer(HearTownCrier);
    context.Transfer(CurfewStartSound);
    context.Transfer(CurfewEndSound);
    context.Transfer(WorkStartSound);
    context.Transfer(WorkEndSound);
    context.Transfer(BribeCost);
    context.Transfer(FramesBetweenGuardSnoop);
    context.Transfer(FramesBetweenGuardSnoopNightShops);
    context.Transfer(FramesBeforeGuardSnoop);
    context.Transfer(FramesBeforeGuardSnoopNightShops);
}