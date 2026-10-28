extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
class CTCBase { public: int tag; };
struct SEntry { long type; CTCBase* component; };
class CThing { public:
    unsigned long UsingComponents[9];
    SEntry entries[8]; int count;
    bool Dead;
    long Type;
    CThing() : count(0), Dead(false), Type(0) { for (int i = 0; i < 9; ++i) UsingComponents[i] = 0; }
    void Add(long type, CTCBase* c)      // keeps the map sorted by type, as CVectorMap does
    {
        int i = count++;
        while (i > 0 && entries[i - 1].type > type) { entries[i] = entries[i - 1]; --i; }
        entries[i].type = type; entries[i].component = c;
        UsingComponents[type >> 5] |= 1UL << (type & 31);
    }
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const { return (UsingComponents[type >> 5] & (1UL << (type & 31))) != 0; }
    template <class T> bool GetTC(long type, T** out) const
    {
        if (!HasTC(type)) return false;
        const SEntry* it = entries; const SEntry* end = entries + count;
        while (it != end && it->type < type) ++it;          // LowerBound
        if (it == end || type < it->type) it = end;
        *out = (it == end) ? 0 : (T*)it->component;
        return true;
    }
};
class CScriptThing { public:
    CThing* thing; bool valid;
    CScriptThing(CThing* t, bool v = true) : thing(t), valid(v) { }
    CThing* GetThing() const { return thing; }
    bool IsValid() const { return valid && thing != 0; }
};
class CPlayer { public: CThing* creature; CThing* GetPControlledCreature() const { return creature; } };
// Behaviour test for 0x00866FB0 CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath::FinishAction: a creature holding
// CTCHero (interface 0x29) gets tutorial category 0x31 (not forced); the base FinishAction always runs afterwards.
class CTCHero : public CTCBase { public: long last; bool forced; int calls; void GiveTutorialForCategory(long c, bool f) { last = c; forced = f; ++calls; } };
static int baseCalls = 0, order = 0, baseOrder = 0, tutOrder = 0;
struct Action {
    CThing* creature;
    void BaseFinishAction() { ++baseCalls; baseOrder = ++order; }
    void FinishAction()
    {
        CTCHero* hero;
        if (creature->GetTC(0x29, &hero)) { hero->GiveTutorialForCategory(0x31, false); tutOrder = ++order; }
        BaseFinishAction();
    }
};
int main()
{
    CThing villager; Action a; a.creature = &villager;
    a.FinishAction(); CHECK(baseCalls == 1 && tutOrder == 0);
    CTCHero h; h.calls = 0; CThing hero; hero.Add(0x04, 0); hero.Add(0x29, &h); a.creature = &hero;
    a.FinishAction();
    CHECK(h.calls == 1 && h.last == 0x31 && !h.forced && baseCalls == 2 && tutOrder < baseOrder);
    if (fails == 0) printf("00866fb0_TEST PASS\n"); else printf("00866fb0_TEST FAIL\n");
    return fails;
}