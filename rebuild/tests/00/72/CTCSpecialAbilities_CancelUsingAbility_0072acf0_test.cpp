// Behaviour test for CTCSpecialAbilities::CancelUsingAbility(EHeroAbility) @ 0x0072ACF0: which TC is cancelled and what is returned.
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
static int g_cancelled = 0;                     // interface type of the TC whose CancelMove ran
class CTCAny : public CTCBase { public: int Type; bool ShieldActive; };
class CTCThing : public CThing { public:
    CTCAny* Slots[0x100];
    CTCThing() : CThing('t') { for (int i = 0; i < 0x100; i++) Slots[i] = 0; }
    template <class T> bool GetTC(T** out)
    {
        CTCAny* tc = Slots[T::StatGetInterfaceType()];
        if (tc) { *out = (T*)tc; return true; }
        return false;
    }
};
#define DECLARE_TC(Name, Type) \
    class Name : public CTCAny { public: \
        static int StatGetInterfaceType() { return Type; } \
        void CancelMove() { g_cancelled = Type; } \
        bool IsShieldActive() { return ShieldActive; } };
DECLARE_TC(CTCPhysicalShieldSpell, 0x93)
DECLARE_TC(CTCSummonSpell, 0x91)
DECLARE_TC(CTCGhostSword, 0x9A)
DECLARE_TC(CTCBulletTime, 0x92)
DECLARE_TC(CTCBerserk, 0x84)
DECLARE_TC(CTCMultiStrike, 0x87)
DECLARE_TC(CTCMultiArrow, 0x88)
typedef int EHeroAbility;
class CTCSpecialAbilities { public:
    CTCThing* ParentThing;
    bool CancelUsingAbility(EHeroAbility ability);
};
bool CTCSpecialAbilities::CancelUsingAbility(EHeroAbility ability)
{
    switch (ability) {
    case 4: {
        CTCPhysicalShieldSpell* shield;
        if (ParentThing->GetTC(&shield) && shield->IsShieldActive()) { shield->CancelMove(); return true; }
        break;
    }
    case 10: { CTCSummonSpell* summon; if (ParentThing->GetTC(&summon)) { summon->CancelMove(); return true; } break; }
    case 15: { CTCGhostSword* sword; if (ParentThing->GetTC(&sword)) { sword->CancelMove(); return true; } break; }
    case 2: { CTCBulletTime* bulletTime; if (ParentThing->GetTC(&bulletTime)) bulletTime->CancelMove(); break; }
    case 1: case 3: case 5: case 6: case 7: case 8: case 11: {
        CTCBerserk* berserk; if (ParentThing->GetTC(&berserk)) berserk->CancelMove(); break;
    }
    case 9: { CTCMultiStrike* multiStrike; if (ParentThing->GetTC(&multiStrike)) multiStrike->CancelMove(); break; }
    case 12: case 13: case 16: case 17: {
        CTCMultiArrow* multiArrow; if (ParentThing->GetTC(&multiArrow)) multiArrow->CancelMove(); break;
    }
    }
    return false;
}
int main()
{
    static CTCThing thing; CTCAny tcs[0x100];
    CTCSpecialAbilities sa; sa.ParentThing = &thing;
    // nothing attached: nothing cancels, always false
    for (int a = 0; a <= 20; a++) { g_cancelled = 0; CHECK(!sa.CancelUsingAbility(a) && g_cancelled == 0); }
    static const int types[] = { 0x93, 0x91, 0x9A, 0x92, 0x84, 0x87, 0x88 };
    for (int i = 0; i < 7; i++) { tcs[types[i]].ShieldActive = false; thing.Slots[types[i]] = &tcs[types[i]]; }
    // expected TC per ability (0 = none) and whether it reports true
    static const int want[20] = { 0, 0x84, 0x92, 0x84, 0, 0x84, 0x84, 0x84, 0x84, 0x87, 0x91, 0x84, 0x88, 0x88, 0, 0x9A, 0x88, 0x88, 0, 0 };
    for (int a = 0; a < 20; a++) {
        g_cancelled = 0; bool r = sa.CancelUsingAbility(a);
        CHECK(g_cancelled == want[a]);
        CHECK(r == (a == 10 || a == 15));
    }
    tcs[0x93].ShieldActive = true; g_cancelled = 0;            // an active shield is cancelled and reported
    CHECK(sa.CancelUsingAbility(4) && g_cancelled == 0x93);
    if (fails) { printf("0072acf0_TEST FAIL\n"); return 1; }
    printf("0072acf0_TEST PASS\n"); return 0;
}