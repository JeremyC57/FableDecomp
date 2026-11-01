// Behaviour test for NScript::CV_FishingCompetitionScript::CFishCompOrganiser::~CFishCompOrganiser (implicit): FishDef, RodDef, CompetitionEntry, csargs, seh_Organiser, seh_Hero, then ~CEntityScriptBase.
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
struct CFishCompOrganiser : public CEntityScriptBase {
    void* ParentClass; void* PMasterData;
    CScriptGameResourceObjectScriptedThingBase seh_Hero, seh_Organiser;
    CsArgsMap csargs;
    bool CommentHitByHero; long Conversation, FishingRodsInGame, FishTypesInGame, FishingPrizes;
    CCharString CompetitionEntry;
    CharStringVector RodDef;
    CharStringVector FishDef;
};
int main()
{
    CFishCompOrganiser* s = new CFishCompOrganiser;
    s->seh_Hero.PImp.Attach(1, 1);
    s->seh_Organiser.PImp.Attach(2, 1);
    s->csargs.NodeCount = 2;
    s->CompetitionEntry.Id = 5;
    s->RodDef.Init(10, 2);
    s->FishDef.Init(20, 3);
    delete s;
    const int want[] = { 4020, 4021, 4022, 4010, 4011, 4005, 3002, 3999, 1002, 1001, 9001 };
    if (!Expect(want, sizeof(want) / sizeof(want[0]))) { std::printf("00e7f210_TEST FAIL\n"); return 1; }
    std::printf("00e7f210_TEST PASS\n");
    return 0;
}