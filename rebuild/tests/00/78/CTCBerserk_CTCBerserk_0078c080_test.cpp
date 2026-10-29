// Behaviour test for CTCBerserk::~CTCBerserk() @ 0x0078C080: kill the live start-up and body effects.
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
class CTCBerserk : public CTCBase { public:
    long Timer;
    CVec<int>                   DependentThings;
    CIntelligentPointer<CThing> StartUpEffect;
    CIntelligentPointer<CThing> BerserkBodyEffect;
    CDefPointer<int>            PDef;
    virtual ~CTCBerserk();
};
CTCBerserk::~CTCBerserk()
{
    if (IS_THING_ALIVE(StartUpEffect))
        StartUpEffect->Kill(true);
    if (IS_THING_ALIVE(BerserkBodyEffect))
        BerserkBodyEffect->Kill(true);
}
int main()
{
    CThing a('a'), b('b'); CDefPointeeBase def;
    reset(); { CTCBerserk* t = new CTCBerserk; t->StartUpEffect.P = &a; t->BerserkBodyEffect.P = &b; t->PDef.Object = &def; delete t; }
    CHECK(strcmp(g_ev, "abDppB") == 0);
    a.Dead = true;
    reset(); { CTCBerserk* t = new CTCBerserk; t->StartUpEffect.P = &a; t->BerserkBodyEffect.P = &b; delete t; }
    CHECK(strcmp(g_ev, "bppB") == 0);
    reset(); { CTCBerserk* t = new CTCBerserk; t->StartUpEffect.P = &b; delete t; }
    CHECK(strcmp(g_ev, "bppB") == 0);
    if (fails) { printf("0078c080_TEST FAIL\n"); return 1; }
    printf("0078c080_TEST PASS\n"); return 0;
}