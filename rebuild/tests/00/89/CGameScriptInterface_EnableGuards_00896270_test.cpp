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
// Behaviour test for 0x00896270 CGameScriptInterface::EnableGuards: forwards the flag to CTCVillage::EnableGuards on a live village thing.
class CTCVillage : public CTCBase { public: int calls; bool flag; const char* str; long timer;
    void SetVillageLimbo(bool b) { ++calls; flag = b; }
    void EnableGuards(bool b) { ++calls; flag = b; }
    void EnableVillagerDefTypes(bool b, const char* s) { ++calls; flag = b; str = s; }
    void ClearHeroEnemyOfGuards() { ++calls; timer = 0; } };
static void Run(const CScriptThing& v, bool b)
{
    CThing* t = v.GetThing(); CTCVillage* tc;
    if (t && !t->IsDead() && t->GetTC(0x22, &tc)) tc->EnableGuards(b);
}
int main()
{
    CTCVillage tc; tc.calls = 0; tc.flag = false; tc.str = 0; tc.timer = 50;
    CThing village; village.Add(0x22, &tc); CThing other;
    Run(CScriptThing(&village), true); CHECK(tc.calls == 1 && tc.flag);
    Run(CScriptThing(&other), false); Run(CScriptThing(0), false);
    village.Dead = true; Run(CScriptThing(&village), false); CHECK(tc.calls == 1);
    if (fails == 0) printf("00896270_TEST PASS\n"); else printf("00896270_TEST FAIL\n");
    return fails;
}