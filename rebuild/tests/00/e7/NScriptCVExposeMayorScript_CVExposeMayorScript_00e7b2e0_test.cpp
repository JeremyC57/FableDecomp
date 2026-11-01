// Behaviour test for NScript::CV_ExposeMayorScript::~CV_ExposeMayorScript (implicit): Evidence, seh_Rhodri, seh_Sheriff, seh_LadyGrey, seh_Suspicious, then ~CScriptBase.
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
struct CV_ExposeMayorScript : public CScriptBase {
    void* Game; void* MasterScriptData;
    long Path, StageNumber; bool Flags[6];
    CScriptGameResourceObjectScriptedThingBase seh_Suspicious, seh_LadyGrey, seh_Sheriff, seh_Rhodri;
    CCharString Evidence;
    float MaxFlashDistFromStables;
};
int main()
{
    CV_ExposeMayorScript* s = new CV_ExposeMayorScript;
    s->seh_Suspicious.PImp.Attach(1, 1);
    s->seh_LadyGrey.PImp.Attach(2, 2);          // still shared: no delete
    s->seh_Sheriff.PImp.Attach(3, 1);
    s->seh_Rhodri.PImp.Attach(4, 1);
    s->Evidence.Id = 7;
    delete s;
    const int want[] = { 4007, 1004, 1003, 1001, 9000 };
    if (!Expect(want, sizeof(want) / sizeof(want[0]))) { std::printf("00e7b2e0_TEST FAIL\n"); return 1; }
    std::printf("00e7b2e0_TEST PASS\n");
    return 0;
}