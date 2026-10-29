// Behaviour test for CTCLook::~CTCLook() @ 0x00753290: unregister from the parent's graphic appearance unless the parent is dead.
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
class CAGraphicAppearanceComponent { public: virtual ~CAGraphicAppearanceComponent() { ev('A'); } };
static const CAGraphicAppearanceComponent* g_unregistered = 0;
class CTCGraphicAppearance { public:
    void UnregisterComponent(CAGraphicAppearanceComponent& c) { g_unregistered = &c; ev('U'); }
};
class CLookThing : public CThing { public:
    CTCGraphicAppearance* PTCGraphicAppearance;
    CLookThing() : CThing('t'), PTCGraphicAppearance(0) {}
};
class CCountedPointerModel { public: ~CCountedPointerModel() { ev('c'); } };
class CTCLook : public CTCBase, public CAGraphicAppearanceComponent { public:
    CDefPointer<int>      PDef;
    CDefPointer<int>      PAppearanceDef;
    CVec<int>             TurningAnims;
    CCountedPointerModel  PHeadTurnAnimH;
    CCountedPointerModel  PHeadTurnAnimV;
    CLookThing* Parent() const { return (CLookThing*)ParentThing; }
    virtual ~CTCLook();
};
CTCLook::~CTCLook()
{
    if (!Parent()->IsDead())
        Parent()->PTCGraphicAppearance->UnregisterComponent(*this);
}
int main()
{
    CLookThing thing; CTCGraphicAppearance app; thing.PTCGraphicAppearance = &app;
    reset(); { CTCLook* t = new CTCLook; t->ParentThing = &thing; CAGraphicAppearanceComponent* c = t; delete t; CHECK(g_unregistered == c); }
    CHECK(strcmp(g_ev, "UccAB") == 0);       // unregister, the two counted pointers, then the bases in reverse order
    thing.Dead = true; g_unregistered = 0;
    reset(); { CTCLook* t = new CTCLook; t->ParentThing = &thing; delete t; }
    CHECK(strcmp(g_ev, "ccAB") == 0 && g_unregistered == 0);
    if (fails) { printf("00753290_TEST FAIL\n"); return 1; }
    printf("00753290_TEST PASS\n"); return 0;
}