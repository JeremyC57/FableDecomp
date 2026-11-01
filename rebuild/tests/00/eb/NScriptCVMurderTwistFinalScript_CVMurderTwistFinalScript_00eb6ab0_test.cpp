// Behaviour test for NScript::CV_MurderTwistFinalScript::~CV_MurderTwistFinalScript (implicit): seh_Guards, seh_Sheriff, Guards, Sheriff in reverse member order, then ~CScriptBase.
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

struct CV_MurderTwistFinalScript : public CScriptBase {
    void* Game; void* MasterScriptData;
    long SheriffState, AssassinState; bool TaskDeclined, PreemptiveAttackInProgress;
    CScriptThing Sheriff;
    Vec<CScriptThing> Guards;
    CScriptGameResourceObjectScriptedThingBase seh_Sheriff;
    Vec<CScriptGameResourceObjectScriptedThingBase> seh_Guards;
};
int main()
{
    CV_MurderTwistFinalScript* q = new CV_MurderTwistFinalScript;
    q->Guards.Init(1); q->seh_Guards.Init(2);
    Attach(q->Sheriff.PImp, 15, 1);
    Attach(q->Guards._M_start[0].PImp, 16, 1);
    Attach(q->seh_Sheriff.PImp, 17, 1);
    Attach(q->seh_Guards._M_start[0].PImp, 18, 1);
    Attach(q->seh_Guards._M_start[1].PImp, 19, 1);
    delete q;
    static const int want[] = { 1018, 1019, 1017, 1016, 1015, 9000 };
    if (!Expect(want, (int)(sizeof(want) / sizeof(want[0])))) { std::printf("00eb6ab0_TEST FAIL\n"); return 1; }
    std::printf("00eb6ab0_TEST PASS\n");
    return 0;
}