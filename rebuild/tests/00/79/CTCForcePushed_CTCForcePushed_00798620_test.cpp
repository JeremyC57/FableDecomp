// Behaviour test for CTCForcePushed::~CTCForcePushed() @ 0x00798620: kill a live push particle, then the members and the base.
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
class CTCForcePushed : public CTCBase { public:
    CIntelligentPointer<CThing> PThingCausingForcePush;
    CIntelligentPointer<CThing> PParticle;
    bool Initialised, StartedMoving;
    CDefPointer<int>            PDef;
    CVec<CThing*>               Emitters_created;
    virtual ~CTCForcePushed();
};
CTCForcePushed::~CTCForcePushed()
{
    if (IS_THING_ALIVE(PParticle))
        PParticle->Kill(true);
}
int main()
{
    CThing particle('k'); CDefPointeeBase def; def.IVRefCount = 1; CThing* block[2] = { 0, 0 };
    reset(); { CTCForcePushed* t = new CTCForcePushed; t->PParticle.P = &particle; t->PDef.Object = &def;
      t->Emitters_created.Start = block; t->Emitters_created.Finish = block + 2; delete t; }
    CHECK(strcmp(g_ev, "kFDppB") == 0);                     // kill, vector block, def (last ref), two pointers, base
    particle.Dead = true; def.IVRefCount = 2;
    reset(); { CTCForcePushed* t = new CTCForcePushed; t->PParticle.P = &particle; t->PDef.Object = &def; delete t; }
    CHECK(strcmp(g_ev, "ppB") == 0 && def.IVRefCount == 1); // dead particle: no kill; shared def: no delete
    reset(); { CTCForcePushed* t = new CTCForcePushed; delete t; }
    CHECK(strcmp(g_ev, "ppB") == 0);                        // null particle
    if (fails) { printf("00798620_TEST FAIL\n"); return 1; }
    printf("00798620_TEST PASS\n"); return 0;
}