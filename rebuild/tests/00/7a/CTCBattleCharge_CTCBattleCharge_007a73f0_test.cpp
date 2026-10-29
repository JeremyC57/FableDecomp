// Behaviour test for CTCBattleCharge::~CTCBattleCharge() @ 0x007A73F0: FadeOrKillEffect on the shield and both feet effects.
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
class CTCSpecialAbilities { public:
    static void FadeOrKillEffect(CIntelligentPointer<CThing>& effect) { ev(effect.P ? effect.P->Tag : '0'); }
};
class CTCBattleCharge : public CTCBase { public:
    CIntelligentPointer<CThing> PTarget;
    CDefPointer<int>            PDef;
    CIntelligentPointer<CThing> CollidedThing;
    CIntelligentPointer<CThing> PItemRemoved;
    CIntelligentPointer<CThing> ShieldEffect;
    CIntelligentPointer<CThing> LeftFeetEffect;
    CIntelligentPointer<CThing> RightFeetEffect;
    CVec<int>                   PeopleHit;
    virtual ~CTCBattleCharge();
};
CTCBattleCharge::~CTCBattleCharge()
{
    CTCSpecialAbilities::FadeOrKillEffect(ShieldEffect);
    CTCSpecialAbilities::FadeOrKillEffect(LeftFeetEffect);
    CTCSpecialAbilities::FadeOrKillEffect(RightFeetEffect);
}
int main()
{
    CThing s('s'), l('l'), r('r'); r.Dead = true; CDefPointeeBase def;
    reset(); { CTCBattleCharge* t = new CTCBattleCharge; t->ShieldEffect.P = &s; t->LeftFeetEffect.P = &l; t->RightFeetEffect.P = &r; t->PDef.Object = &def; delete t; }
    CHECK(strcmp(g_ev, "slrpppppDpB") == 0);   // the helper sees every effect (alive or not), then 6 pointers + def in reverse order
    reset(); { CTCBattleCharge* t = new CTCBattleCharge; delete t; }
    CHECK(strcmp(g_ev, "000ppppppB") == 0);
    if (fails) { printf("007a73f0_TEST FAIL\n"); return 1; }
    printf("007a73f0_TEST PASS\n"); return 0;
}