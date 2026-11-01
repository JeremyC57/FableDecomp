// Behaviour test for NScript::CV_ChickenKickingScript::~CV_ChickenKickingScript (implicit): seh_ChickenMaster, seh_Chicken, then the csargs map clear, then ~CScriptBase.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_log[64];
static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
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
static void DeleteData(void* p) { Log(1000 + *(int*)p); }
template <class T> class CCountedPointer { public:
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
};
static int g_ids[32] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };
template <class T> static void Attach(CCountedPointer<T>& p, int id, unsigned long refs)
{
    p.Info = new CCPPointerInfo;
    p.Info->RefCount = refs;
    p.Info->DeleteFunc = DeleteData;
    p.Info->Data = &g_ids[id];
}
class CBaseClass { public: virtual ~CBaseClass() { } };
class CScriptThing : public CBaseClass { public:
    CCountedPointer<CScriptThing> PImp;
    virtual ~CScriptThing() { }
};
class CTimer { public:
    long TimerIndex;
    ~CTimer() { Log(2000 + TimerIndex); }     // DeregisterTimer(TimerIndex) through the script interface
};
class CScriptGameResourceObjectScriptedThingBase { public:
    void* IntelligentPointer;
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;
    virtual ~CScriptGameResourceObjectScriptedThingBase() { }
};
template <class T> struct Vec {                 // STLport vector shape: destroy [start, finish) forward, then free(start)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    Vec() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    void Init(int n) { _M_start = (T*)std::malloc(sizeof(T) * n); for (int i = 0; i < n; ++i) new (&_M_start[i]) T(); _M_finish = _M_end_of_storage = _M_start + n; }
    ~Vec() { for (T* p = _M_start; p != _M_finish; ++p) p->~T(); if (_M_start != 0) std::free(_M_start); }
};
class CScriptBase { public: virtual ~CScriptBase() { Log(9000); } unsigned char _body[0x3c]; };
class CEntityScriptBase { public: virtual ~CEntityScriptBase() { Log(9001); } void* Game; CScriptThing Me; };

struct CsArgsMap {                               // stands in for map<CCharString,CCharString>: clear() erases the nodes
    unsigned int _M_node_count;
    ~CsArgsMap() { if (_M_node_count != 0) { Log(3000 + (int)_M_node_count); _M_node_count = 0; } }
};
struct CV_ChickenKickingScript : public CScriptBase {
    void* Game; void* MasterScriptData;
    bool ChickenLanded; long FinalSector, DistanceBand; bool Flags[5]; unsigned long PrizesWon; bool InfoDisplayed;
    CsArgsMap csargs;
    CScriptGameResourceObjectScriptedThingBase seh_Chicken, seh_ChickenMaster;
};
int main()
{
    CV_ChickenKickingScript* q = new CV_ChickenKickingScript;
    q->csargs._M_node_count = 2;
    Attach(q->seh_Chicken.PImp, 21, 1);
    Attach(q->seh_ChickenMaster.PImp, 22, 1);
    delete q;
    static const int want[] = { 1022, 1021, 3002, 9000 };
    if (!Expect(want, (int)(sizeof(want) / sizeof(want[0])))) { std::printf("00e68eb0_TEST FAIL\n"); return 1; }
    std::printf("00e68eb0_TEST PASS\n");
    return 0;
}