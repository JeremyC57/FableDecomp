// Behaviour test for 0x004E1E06 (copy of the model + stand-in callee bodies).
#include <cstdio>
#include <cstring>
// CVillageDef::operator=(const CVillageDef&) -- memberwise def assignment: CDefClassBase part, the village task vector, the routine start-time vectors and durations, sleep/curfew/lighting times, the three flags, the four curfew/work sound names and the guard-snoop timings; returns *this
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E1E06, length 537 bytes; size-optimised def TU.
// Callees: CDefClassBase::operator= @ 0x00431F10, vector<EVillageTaskType>::operator= @ 0x004E201F,
//          vector<long>::operator= @ 0x00454886 (x7), CCharString::operator= @ 0x0099EFB0 (x4).
// Identity: reached through the jmp thunk 0x004E1E01 in slot 19 of vtable 0x01241DDC = CVillageDef
// (rtti-vtables.tsv); the manifest labels this row "CThingBuildingDef::operator=_full" by propagation.
// Layout: PDB CVillageDef (Ego_r) uses 16-byte vectors; the retail STLport vectors are 12 bytes, so
// every member after Tasks sits 4 bytes lower per preceding vector (PatrolTrackName +0x34 ..
// GamingDuration +0xb0, the plain times +0xb4..+0xe0, the bools +0xe4..+0xe6, the sound names
// +0xe8..+0xf4, the snoop timings +0xf8..+0x108) -- exactly the offsets the body uses.
// Shape: memberwise assignment in declaration order; dword members copy through eax.

class CDefClassBase { public:
    unsigned char _body[0x28];          // vptr, resource links, PDefManager, GlobalIndex, flag bits
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

class CDefString { public:                     // a def-string table index (4 bytes, trivially copied)
    long Index;
};

struct CVillageTaskTypeVector {         // retail std::vector<EVillageTaskType> (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CVillageTaskTypeVector& operator=(const CVillageTaskTypeVector& other);   // 0x004E201F
};

struct CLongVector {                    // retail std::vector<long> (12 bytes)
    long* m_first;
    long* m_last;
    long* m_end;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

class CCharString { public:                    // ref-counted string handle (4 bytes)
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CVillageDef : public CDefClassBase { public:                                // vtable 0x01241DDC
    CVillageTaskTypeVector Tasks;               // +0x28
    CDefString PatrolTrackName;                 // +0x34
    long DefaultStanceToHero;                   // +0x38 EVillageStanceToHero
    long SecondsSecuritySweepLasts;             // +0x3c
    long WarehouseCrateDef;                     // +0x40
    CLongVector StoryStartTimes;                // +0x44
    long StoryDuration;                         // +0x50
    CLongVector SchoolStartTimes;               // +0x54
    long SchoolDuration;                        // +0x60
    CLongVector WorkStartTimes;                 // +0x64
    long WorkDuration;                          // +0x70
    CLongVector CookStartTimes;                 // +0x74
    long CookDuration;                          // +0x80
    CLongVector EatStartTimes;                  // +0x84
    long EatDuration;                           // +0x90
    CLongVector TavernStartTimes;               // +0x94
    long TavernDuration;                        // +0xa0
    CLongVector GamingStartTimes;               // +0xa4
    long GamingDuration;                        // +0xb0
    long SleepAdultStartTime;                   // +0xb4
    long SleepAdultEndTime;                     // +0xb8
    long SleepChildStartTime;                   // +0xbc
    long SleepChildEndTime;                     // +0xc0
    long SleepElderlyStartTime;                 // +0xc4
    long SleepElderlyEndTime;                   // +0xc8
    long HouseLightsOnTime;                     // +0xcc
    long HouseLightsOffTime;                    // +0xd0
    long SociableStartTime;                     // +0xd4
    long SociableFinishTime;                    // +0xd8
    long CurfewStartTime;                       // +0xdc
    long CurfewFinishTime;                      // +0xe0
    bool EnableAutoPlacement;                   // +0xe4
    bool AllowHouseBuying;                      // +0xe5
    bool HearTownCrier;                         // +0xe6
    CCharString CurfewStartSound;               // +0xe8
    CCharString CurfewEndSound;                 // +0xec
    CCharString WorkStartSound;                 // +0xf0
    CCharString WorkEndSound;                   // +0xf4
    long BribeCost;                             // +0xf8
    long FramesBetweenGuardSnoop;               // +0xfc
    long FramesBetweenGuardSnoopNightShops;     // +0x100
    long FramesBeforeGuardSnoop;                // +0x104
    long FramesBeforeGuardSnoopNightShops;      // +0x108
    CVillageDef& operator=(const CVillageDef& other);
};

CVillageDef& CVillageDef::operator=(const CVillageDef& other)
{
    CDefClassBase::operator=(other);
    Tasks = other.Tasks;
    PatrolTrackName = other.PatrolTrackName;
    DefaultStanceToHero = other.DefaultStanceToHero;
    SecondsSecuritySweepLasts = other.SecondsSecuritySweepLasts;
    WarehouseCrateDef = other.WarehouseCrateDef;
    StoryStartTimes = other.StoryStartTimes;
    StoryDuration = other.StoryDuration;
    SchoolStartTimes = other.SchoolStartTimes;
    SchoolDuration = other.SchoolDuration;
    WorkStartTimes = other.WorkStartTimes;
    WorkDuration = other.WorkDuration;
    CookStartTimes = other.CookStartTimes;
    CookDuration = other.CookDuration;
    EatStartTimes = other.EatStartTimes;
    EatDuration = other.EatDuration;
    TavernStartTimes = other.TavernStartTimes;
    TavernDuration = other.TavernDuration;
    GamingStartTimes = other.GamingStartTimes;
    GamingDuration = other.GamingDuration;
    SleepAdultStartTime = other.SleepAdultStartTime;
    SleepAdultEndTime = other.SleepAdultEndTime;
    SleepChildStartTime = other.SleepChildStartTime;
    SleepChildEndTime = other.SleepChildEndTime;
    SleepElderlyStartTime = other.SleepElderlyStartTime;
    SleepElderlyEndTime = other.SleepElderlyEndTime;
    HouseLightsOnTime = other.HouseLightsOnTime;
    HouseLightsOffTime = other.HouseLightsOffTime;
    SociableStartTime = other.SociableStartTime;
    SociableFinishTime = other.SociableFinishTime;
    CurfewStartTime = other.CurfewStartTime;
    CurfewFinishTime = other.CurfewFinishTime;
    EnableAutoPlacement = other.EnableAutoPlacement;
    AllowHouseBuying = other.AllowHouseBuying;
    HearTownCrier = other.HearTownCrier;
    CurfewStartSound = other.CurfewStartSound;
    CurfewEndSound = other.CurfewEndSound;
    WorkStartSound = other.WorkStartSound;
    WorkEndSound = other.WorkEndSound;
    BribeCost = other.BribeCost;
    FramesBetweenGuardSnoop = other.FramesBetweenGuardSnoop;
    FramesBetweenGuardSnoopNightShops = other.FramesBetweenGuardSnoopNightShops;
    FramesBeforeGuardSnoop = other.FramesBeforeGuardSnoop;
    FramesBeforeGuardSnoopNightShops = other.FramesBeforeGuardSnoopNightShops;
    return *this;
}

CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other) { std::memcpy(_body, other._body, sizeof(_body)); return *this; }
CVillageTaskTypeVector& CVillageTaskTypeVector::operator=(const CVillageTaskTypeVector& other) { m_first = other.m_first; m_last = other.m_last; m_end = other.m_end; return *this; }
CLongVector& CLongVector::operator=(const CLongVector& other) { m_first = other.m_first; m_last = other.m_last; m_end = other.m_end; return *this; }
CCharString& CCharString::operator=(const CCharString& other) { m_pData = other.m_pData; return *this; }

int main()
{
    static CVillageDef a, b;
    long x = 0;
    b._body[1] = 4; b.Tasks.m_end = &b; b.PatrolTrackName.Index = 12; b.GamingStartTimes.m_first = &x;
    b.GamingDuration = 30; b.CurfewFinishTime = 22; b.HearTownCrier = true; b.WorkEndSound.m_pData = &x;
    b.FramesBeforeGuardSnoopNightShops = 600;
    CVillageDef* r = &(a = b);
    if (r != &a || a._body[1] != 4 || a.Tasks.m_end != &b || a.PatrolTrackName.Index != 12
        || a.GamingStartTimes.m_first != &x || a.GamingDuration != 30 || a.CurfewFinishTime != 22
        || !a.HearTownCrier || a.WorkEndSound.m_pData != &x || a.FramesBeforeGuardSnoopNightShops != 600) { std::printf("004e1e06_TEST FAIL\n"); return 1; }
    std::printf("004e1e06_TEST PASS\n");
    return 0;
}