// Behaviour test for NCreatureMode::CModeBriarRoseConcentrate implicit dtor @ 0x00743120: members in reverse order, then the base.
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
class CModeStandBase { public: virtual ~CModeStandBase() { ev('B'); } };
class CAnimFilterBase { public: virtual ~CAnimFilterBase() { ev('f'); } };
class CAnimFilterOfGroup : public CAnimFilterBase { public: CVec<int> Groups; };
namespace NCreatureMode {
class CModeBriarRoseConcentrate : public CModeStandBase { public:   // implicit (compiler-generated) destructor
    CAnimFilterOfGroup                      Filter;
    long                                    Timer;
    CIntelligentPointer<CThingCreatureBase> PTarget;
    CVec<CIntelligentPointer<CThing> >      Imposters;
    bool                                    Imposter;
    CDefPointer<int>                        PDef;
    CIntelligentPointer<CThing>             PBodyEffect;
    CIntelligentPointer<CThing>             PSwordEffect;
};
}
int main()
{
    CDefPointeeBase def;
    CIntelligentPointer<CThing>* imp = (CIntelligentPointer<CThing>*)malloc(2 * sizeof(CIntelligentPointer<CThing>));
    reset(); { CModeStandBase* m = new NCreatureMode::CModeBriarRoseConcentrate;
      NCreatureMode::CModeBriarRoseConcentrate* b = (NCreatureMode::CModeBriarRoseConcentrate*)m;
      b->PDef.Object = &def; b->Imposters.Start = imp; b->Imposters.Finish = imp + 2; delete m; }
    CHECK(strcmp(g_ev, "ppDppFpfB") == 0);   // sword, body, def, the two imposters + block, target, filter, base
    free(imp);
    if (fails) { printf("00743120_TEST FAIL\n"); return 1; }
    printf("00743120_TEST PASS\n"); return 0;
}