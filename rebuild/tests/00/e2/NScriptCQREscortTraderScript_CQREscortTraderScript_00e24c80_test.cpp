// Behaviour test for NScript::CQR_EscortTraderScript::~CQR_EscortTraderScript (implicit): CurrentLoadedRegion, PreviousLoadedRegion, RouteStatic[3], RouteDynamic[3], RandomSpeechWait[5], RandomSpeechFollow[10], the four timers, BonusObject, then ~CScriptBase.
#include <cstdio>
#include <cstdlib>
static int g_log[128];
static int g_n = 0;
static void Log(int v) { if (g_n < 128) g_log[g_n++] = v; }
static bool Expect(const int* want, int n)
{
    if (g_n != n) return false;
    for (int i = 0; i < n; ++i) if (g_log[i] != want[i]) return false;
    return true;
}
class CCPPointerInfo { public:
    unsigned long RefCount;
    void (*DeleteFunc)(void*);
    void* Data;
};
static int g_ids[64];
static void DeleteData(void* p) { Log(1000 + *(int*)p); }
template <class T> class CCountedPointer { public:      // engine/CCountedPointer.h release: count down, DeleteFunc, free the block
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    void Attach(int id, unsigned long refs)
    {
        g_ids[id] = id;
        Info = new CCPPointerInfo;
        Info->RefCount = refs;
        Info->DeleteFunc = DeleteData;
        Info->Data = &g_ids[id];
    }
};
class CBaseClass { public: virtual ~CBaseClass() { } };
class CScriptThing : public CBaseClass { public:          // ~CScriptThing: release PImp, then ~CBaseClass
    CCountedPointer<CScriptThing> PImp;
    virtual ~CScriptThing() { }
};
class CScriptGameResourceObjectScriptedThingBase {        // implicit dtor: release PImp, base vtable, ~CBaseObject
public:
    void* Base;
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;
    virtual ~CScriptGameResourceObjectScriptedThingBase() { }
};
class CTimer { public:                                    // ~CTimer: DeregisterTimer(TimerIndex)
    long TimerIndex;
    CTimer() : TimerIndex(0) { }
    ~CTimer() { Log(2000 + TimerIndex); }
};
class CCharString { public:                               // ~CCharString 0x0099EAE0
    int Id;
    CCharString() : Id(0) { }
    ~CCharString() { Log(4000 + Id); }
};
struct CsArgsMap {                                        // map<CCharString,CCharString>: clear (erase the nodes) then free the header
    unsigned int NodeCount;
    CsArgsMap() : NodeCount(0) { }
    ~CsArgsMap() { if (NodeCount != 0) { Log(3000 + (int)NodeCount); NodeCount = 0; } Log(3999); }
};
struct ScriptThingList {                                  // list<CScriptThing>: out-of-line clear, then free the sentinel
    int Id;
    ScriptThingList() : Id(0) { }
    ~ScriptThingList() { Log(5000 + Id); }
};
struct CharStringVector {                                 // STLport vector<CCharString>: destroy [start, finish), free start
    CCharString* Start;
    CCharString* Finish;
    CharStringVector() : Start(0), Finish(0) { }
    void Init(int firstId, int n)
    {
        Start = (CCharString*)std::malloc(sizeof(CCharString) * n);
        for (int i = 0; i < n; ++i) { Start[i].Id = firstId + i; }
        Finish = Start + n;
    }
    ~CharStringVector()
    {
        for (CCharString* p = Start; p != Finish; ++p) p->~CCharString();
        if (Start != 0) std::free(Start);
    }
};
class CScriptBase { public: virtual ~CScriptBase() { Log(9000); } unsigned char _body[0x3c]; };
class CEntityScriptBase { public: virtual ~CEntityScriptBase() { Log(9001); } void* Game; CScriptThing Me; };
struct CQR_EscortTraderScript : public CScriptBase {
    void* Game; void* MasterScriptData;
    bool Flags1[10]; long BanditSpawnCount, BanditKillCount; bool Flags2[8];
    long TraderHitCount, TraderHealthBarID; float TraderInitialHealth; bool Flags3[3];
    CCharString BonusObject;
    CTimer TraderComplainTimer, TraderExpressionDetectTimer, TraderHitDetectTimer, TraderRandomSpeechTimer;
    CCharString RandomSpeechFollow[10];
    CCharString RandomSpeechWait[5];
    long TradeRoute;
    CharStringVector RouteDynamic[3];
    CharStringVector RouteStatic[3];
    CCharString PreviousLoadedRegion, CurrentLoadedRegion;
    bool RegionChangeDetected; long QuestStage;
};
int main()
{
    CQR_EscortTraderScript* s = new CQR_EscortTraderScript;
    s->BonusObject.Id = 1;
    s->TraderComplainTimer.TimerIndex = 1; s->TraderExpressionDetectTimer.TimerIndex = 2;
    s->TraderHitDetectTimer.TimerIndex = 3; s->TraderRandomSpeechTimer.TimerIndex = 4;
    for (int i = 0; i < 10; ++i) s->RandomSpeechFollow[i].Id = 10 + i;
    for (int j = 0; j < 5; ++j) s->RandomSpeechWait[j].Id = 30 + j;
    s->RouteDynamic[1].Init(50, 2);
    s->RouteStatic[0].Init(60, 1);
    s->RouteStatic[2].Init(70, 2);
    s->PreviousLoadedRegion.Id = 90; s->CurrentLoadedRegion.Id = 91;
    delete s;
    const int want[] = { 4091, 4090, 4070, 4071, 4060, 4050, 4051,
                         4034, 4033, 4032, 4031, 4030,
                         4019, 4018, 4017, 4016, 4015, 4014, 4013, 4012, 4011, 4010,
                         2004, 2003, 2002, 2001, 4001, 9000 };
    if (!Expect(want, sizeof(want) / sizeof(want[0]))) { std::printf("00e24c80_TEST FAIL\n"); return 1; }
    std::printf("00e24c80_TEST PASS\n");
    return 0;
}