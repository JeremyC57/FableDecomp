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
// Behaviour test for 0x00898f40 CGameScriptInterface::GiveHeroGold: AddMoney(amount, true) on a live hero holding CTCHeroStats; nothing otherwise.
class CTCHeroStats : public CTCBase { public: long Money; long TimesHadSex; bool FailQuestOnDeath; void AddMoney(long m, bool) { Money += m; } };
static CPlayer mainPlayer;
static void Run(long amount) { CThing* hero = mainPlayer.GetPControlledCreature(); CTCHeroStats* s; if (hero && !hero->IsDead() && hero->GetTC(0x04, &s)) s->AddMoney(amount, true); }
int main()
{
    CTCHeroStats c; CThing h; h.Add(0x29, 0); h.Add(0x04, &c); CThing plain;
    c.Money = 100; mainPlayer.creature = &h; Run(50); CHECK(c.Money == 150);
    Run(-30); CHECK(c.Money == 120);
    h.Dead = true; Run(1000); CHECK(c.Money == 120); h.Dead = false;
    mainPlayer.creature = &plain; Run(5); mainPlayer.creature = 0; Run(5); CHECK(c.Money == 120);
    if (fails == 0) printf("00898f40_TEST PASS\n"); else printf("00898f40_TEST FAIL\n");
    return fails;
}