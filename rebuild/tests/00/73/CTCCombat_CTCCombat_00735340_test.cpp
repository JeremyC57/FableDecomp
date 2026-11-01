// Behaviour test for CTCCombat::~CTCCombat 0x00735340: the body clears the combat-ability map and kills every live
// flourish particle (null and dead particles are skipped); then members release in reverse order before ~CTCBase.
#include <cstdio>
#include <cstring>
static char g_log[256];
static void Log(char c) { std::size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
struct Thing { bool Dead; int Kills; char Tag; void Kill(bool) { ++Kills; Log(Tag); } bool IsDead() const { return Dead; } };
struct IntelligentPointer {
    Thing* P;
    IntelligentPointer() : P(0) { }
    virtual ~IntelligentPointer() { Log('i'); }
    Thing* Get() const { return P; }
    Thing* operator->() const { return P; }
};
static bool IS_THING_ALIVE(const IntelligentPointer& p) { if (p.Get() != 0) { if (!p->IsDead()) return true; } return false; }
struct DefObject { unsigned int IVRefCount; char Tag; void Delete() const { Log(Tag); } };
struct DefPointer {
    DefObject* Object;
    DefPointer() : Object(0) { }
    ~DefPointer() { DefObject* o = Object; if (o) { if (--o->IVRefCount == 0) o->Delete(); Object = 0; } }
};
struct AbilityMap { int Count; void clear() { if (Count != 0) { Log('m'); Count = 0; } } ~AbilityMap() { clear(); Log('M'); } };
struct TCBase { virtual ~TCBase() { Log('B'); } };
struct TCCombat : TCBase {
    DefPointer PCombatTypeDef;
    DefPointer PBlockCounterAbility, PFlourish360Ability;
    AbilityMap CombatAbilities;
    IntelligentPointer FlourishParticles[3];
    long FlourishCount;
    ~TCCombat()
    {
        CombatAbilities.clear();
        for (long i = 0; i < FlourishCount; ++i)
            if (IS_THING_ALIVE(FlourishParticles[i]))
                FlourishParticles[i]->Kill(true);
    }
};
int main()
{
    Thing a = { false, 0, 'a' }, b = { true, 0, 'b' };
    DefObject t = { 1, 't' }, f = { 2, 'f' }, c = { 1, 'c' };
    TCCombat* x = new TCCombat;
    x->PCombatTypeDef.Object = &t; x->PFlourish360Ability.Object = &f; x->PBlockCounterAbility.Object = &c;
    x->CombatAbilities.Count = 2;
    x->FlourishParticles[0].P = &a; x->FlourishParticles[1].P = &b; x->FlourishParticles[2].P = 0;
    x->FlourishCount = 3;
    TCBase* p = x;
    delete p;
    // body: map clear (m), kill a; members: particles (iii), map (no second clear, M), f survives (2 refs), c, t, base B
    if (std::strcmp(g_log, "maiiiMctB") != 0 || a.Kills != 1 || b.Kills != 0 || f.IVRefCount != 1) {
        std::printf("00735340_TEST FAIL %s\n", g_log); return 1;
    }
    std::printf("00735340_TEST PASS\n");
    return 0;
}