// CVillageDef::CVillageDef() -- default def constructor: the parent def base, own vtable, eight empty vectors (tasks and the seven activity start-time lists), PatrolTrackName = -1 and the four sound-name strings
// Leaf: ??0CVillageDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DFF04, length 155 bytes.
// Callees: the def base constructor @ 0x0044C0C0 (manifest "CShoveHaPennyRegionDef", shared by 265 def ctors),
// CCharString::CCharString() @ 0x0099E4B0 (x4).
// Vtable 0x01241DDC (rtti-vtables.tsv: CVillageDef). Layout (retail; Ego_r.pdb with 16-byte debug vectors, so each later member
// sits 4 bytes lower per preceding vector): Tasks +0x28, PatrolTrackName +0x34, DefaultStanceToHero +0x38,
// SecondsSecuritySweepLasts +0x3c, WarehouseCrateDef +0x40, StoryStartTimes +0x44, StoryDuration +0x50, SchoolStartTimes +0x54,
// WorkStartTimes +0x64, CookStartTimes +0x74, EatStartTimes +0x84, TavernStartTimes +0x94, GamingStartTimes +0xa4,
// GamingDuration +0xb0, the sleep/lights/sociable/curfew times +0xb4..+0xe0, the three flags +0xe4, CurfewStartSound +0xe8,
// CurfewEndSound +0xec, WorkStartSound +0xf0, WorkEndSound +0xf4, the bribe/snoop longs +0xf8..+0x108.
// Shape: a size-optimised TU (`#pragma optimize("s", on)`: `or dword ptr [m],-1` for the -1 store and `lea ecx` bases for the
// vectors past +0x80).
#pragma optimize("s", on)
struct CCharString {
    void* PStringData;
    CCharString();                                 // 0x0099E4B0
};
struct CDefString {
    long TablePos;
    CDefString() : TablePos(-1) {}
};
template <class T> struct CRetailVector {          // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CDefBase_0044c0c0 {                         // the parent def base (0x28 bytes)
    virtual void v00();
    unsigned char _body[0x24];
    CDefBase_0044c0c0();                           // 0x0044C0C0
};
struct CVillageDef : CDefBase_0044c0c0 {           // vtable 0x01241DDC
    CRetailVector<long> Tasks;                     // +0x28 vector<EVillageTaskType>
    CDefString          PatrolTrackName;           // +0x34
    long                DefaultStanceToHero;       // +0x38
    long                SecondsSecuritySweepLasts; // +0x3c
    long                WarehouseCrateDef;         // +0x40
    CRetailVector<long> StoryStartTimes;           // +0x44
    long                StoryDuration;             // +0x50
    CRetailVector<long> SchoolStartTimes;          // +0x54
    long                SchoolDuration;            // +0x60
    CRetailVector<long> WorkStartTimes;            // +0x64
    long                WorkDuration;              // +0x70
    CRetailVector<long> CookStartTimes;            // +0x74
    long                CookDuration;              // +0x80
    CRetailVector<long> EatStartTimes;             // +0x84
    long                EatDuration;               // +0x90
    CRetailVector<long> TavernStartTimes;          // +0x94
    long                TavernDuration;            // +0xa0
    CRetailVector<long> GamingStartTimes;          // +0xa4
    long                GamingDuration;            // +0xb0
    long                SleepAdultStartTime;       // +0xb4
    long                SleepAdultEndTime;         // +0xb8
    long                SleepChildStartTime;       // +0xbc
    long                SleepChildEndTime;         // +0xc0
    long                SleepElderlyStartTime;     // +0xc4
    long                SleepElderlyEndTime;       // +0xc8
    long                HouseLightsOnTime;         // +0xcc
    long                HouseLightsOffTime;        // +0xd0
    long                SociableStartTime;         // +0xd4
    long                SociableFinishTime;        // +0xd8
    long                CurfewStartTime;           // +0xdc
    long                CurfewFinishTime;          // +0xe0
    bool                EnableAutoPlacement;       // +0xe4
    bool                AllowHouseBuying;          // +0xe5
    bool                HearTownCrier;             // +0xe6
    CCharString         CurfewStartSound;          // +0xe8
    CCharString         CurfewEndSound;            // +0xec
    CCharString         WorkStartSound;            // +0xf0
    CCharString         WorkEndSound;              // +0xf4
    long                BribeCost;                 // +0xf8
    long                FramesBetweenGuardSnoop;   // +0xfc
    long                FramesBetweenGuardSnoopNightShops;   // +0x100
    long                FramesBeforeGuardSnoop;    // +0x104
    long                FramesBeforeGuardSnoopNightShops;    // +0x108
    virtual void v00();
    CVillageDef();
};
CVillageDef::CVillageDef()
{
}
