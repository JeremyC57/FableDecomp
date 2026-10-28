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
// Behaviour test for 0x00898e60 CGameScriptInterface::IsHeroHandLampLit: the live hero's CTCLight (interface 0x27) active flag; false without a live hero.
class CTCLight : public CTCBase { public: bool Active; bool IsActive() const { return Active; } };
static CPlayer mainPlayer;
static bool Run() { CThing* hero = mainPlayer.GetPControlledCreature(); if (hero && !hero->IsDead()) { CTCLight* l = 0; hero->GetTC(0x27, &l); return l->IsActive(); } return false; }
int main()
{
    CTCLight c; CThing h; h.Add(0x04, 0); h.Add(0x27, &c); CThing plain;
    c.Active = true; mainPlayer.creature = &h; CHECK(Run());
    c.Active = false; CHECK(!Run());
    c.Active = true; h.Dead = true; CHECK(!Run()); mainPlayer.creature = 0; CHECK(!Run());
    if (fails == 0) printf("00898e60_TEST PASS\n"); else printf("00898e60_TEST FAIL\n");
    return fails;
}