// Behaviour test for CTCChest::~CTCChest() @ 0x00750850: kill a live opening particle, then the container base members.
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
class CCharString { public: ~CCharString() { ev('s'); } };
class CTCContainer : public CTCBase { public: CVec<CCharString> ContainerItems; };
class CTCContainerRewardHero : public CTCContainer { public: bool Enabled; };
class COpenCloseHelper { public: bool Opened; float Amount; };
class CTCChest : public CTCContainerRewardHero { public:
    COpenCloseHelper            ChestOpener;
    bool                        ChestOpen;
    CIntelligentPointer<CThing> POpenParticle;
    virtual ~CTCChest();
};
CTCChest::~CTCChest()
{
    if (IS_THING_ALIVE(POpenParticle))
        POpenParticle->Kill(true);
}
int main()
{
    CThing part('k');
    CCharString* items = (CCharString*)malloc(2 * sizeof(CCharString));
    reset(); { CTCChest* t = new CTCChest; t->POpenParticle.P = &part; t->ContainerItems.Start = items; t->ContainerItems.Finish = items + 2; delete t; }
    CHECK(strcmp(g_ev, "kpssFB") == 0);       // kill, the particle pointer, then the container items and their block, base
    part.Dead = true;
    reset(); { CTCChest* t = new CTCChest; t->POpenParticle.P = &part; delete t; }
    CHECK(strcmp(g_ev, "pB") == 0);
    free(items);
    if (fails) { printf("00750850_TEST FAIL\n"); return 1; }
    printf("00750850_TEST PASS\n"); return 0;
}