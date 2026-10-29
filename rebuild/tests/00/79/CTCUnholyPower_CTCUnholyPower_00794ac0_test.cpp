// Behaviour test for CTCUnholyPower::~CTCUnholyPower() @ 0x00794AC0: default theme unless finished, kill a live obstruction.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_ev[256];                       // event trace: one letter per event
static void ev(char c) { size_t n = strlen(g_ev); if (n + 1 < sizeof(g_ev)) { g_ev[n] = c; g_ev[n + 1] = 0; } }
static void reset() { g_ev[0] = 0; }
class CThing { public:
    bool Dead; char Tag;
    CThing(char tag) : Dead(false), Tag(tag) {}
    bool IsDead() const { return Dead; }
    void Kill(bool immediately) { if (immediately) ev(Tag); else ev('?'); }
};
class CThingCreatureBase : public CThing { public: CThingCreatureBase(char t) : CThing(t) {} };
template <class T> class CIntelligentPointer { public:
    T* P;
    CIntelligentPointer() : P(0) {}
    ~CIntelligentPointer() { ev('p'); }
    T* Get() const { return P; }
    T* operator->() const { return P; }
};
template <class T> inline bool IS_THING_ALIVE(const CIntelligentPointer<T>& p)
{
    if (p.Get() != 0) {
        if (!p->IsDead())
            return true;
    }
    return false;
}
class CDefPointeeBase { public:
    mutable unsigned long IVRefCount;
    CDefPointeeBase() : IVRefCount(1) {}
    void Delete() const { ev('D'); }
};
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) {}
    void Release()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    ~CDefPointer() { Release(); }
};
template <class T> class CVec { public:     // STLport vector teardown: destroy the elements, free the block
    T* Start; T* Finish;
    CVec() : Start(0), Finish(0) {}
    ~CVec() { for (T* p = Start; p != Finish; ++p) p->~T(); if (Start) ev('F'); }
};
class CTCBase { public:
    CThing* ParentThing;
    CTCBase() : ParentThing(0) {}
    virtual ~CTCBase() { ev('B'); }
};
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("check failed line %d\n", __LINE__); fails++; } } while (0)
static long g_channelsMode = -1; static float g_time = -1;
class CEnvironment { public:
    void TransitionToDefaultThemeAllChannels(long mode, float time) { g_channelsMode = mode; g_time = time; ev('T'); }
};
class CWorld { public: CEnvironment Env; CEnvironment& GetEnvironment() { return Env; } };
static CWorld g_world;
class CTCWithWorld : public CTCBase { protected: CWorld& GetWorld() const { return g_world; } };
class CTCUnholyPower : public CTCWithWorld { public:
    CDefPointer<int>            PDef;
    bool                        Finished;
    CIntelligentPointer<CThing> PObstruction;
    CTCUnholyPower() : Finished(false) {}
    virtual ~CTCUnholyPower();
};
CTCUnholyPower::~CTCUnholyPower()
{
    if (!Finished)
        GetWorld().GetEnvironment().TransitionToDefaultThemeAllChannels(2, 0.001f);
    if (IS_THING_ALIVE(PObstruction))
        PObstruction->Kill(true);
}
int main()
{
    CThing o('o'); CDefPointeeBase def;
    reset(); { CTCUnholyPower* t = new CTCUnholyPower; t->PObstruction.P = &o; t->PDef.Object = &def; delete t; }
    CHECK(strcmp(g_ev, "TopDB") == 0 && g_channelsMode == 2 && g_time == 0.001f);
    o.Dead = true; g_channelsMode = -1;
    reset(); { CTCUnholyPower* t = new CTCUnholyPower; t->Finished = true; t->PObstruction.P = &o; delete t; }
    CHECK(strcmp(g_ev, "pB") == 0 && g_channelsMode == -1);
    if (fails) { printf("00794ac0_TEST FAIL\n"); return 1; }
    printf("00794ac0_TEST PASS\n"); return 0;
}