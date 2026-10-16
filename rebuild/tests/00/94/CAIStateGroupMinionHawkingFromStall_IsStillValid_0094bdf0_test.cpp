// Behaviour test for 0094bdf0 (see src/0094bdf0.cpp); plain C++98, member access only.
#include <cstdio>
// shared test model: a thing with an interface table (sorted type -> component), a dead bit, and an
// intelligent pointer; mirrors the HasTC / GetInterface / FindEntry shapes of the sources.
struct TThing {
    unsigned long bits[9];
    long types[8];
    void* comps[8];
    int count;
    unsigned char flags91;
    TThing() : count(0), flags91(0) { for (int i = 0; i < 9; ++i) bits[i] = 0; }
    void Add(long type, void* comp) { types[count] = type; comps[count] = comp; ++count; bits[type >> 5] |= 1UL << (type & 31); }
    unsigned long HasTC(long type) const { return bits[type >> 5] & (1UL << (type & 31)); }
    void* Find(long type) const { for (int i = 0; i < count; ++i) if (types[i] == type) return comps[i]; return 0; }
    template<class T> bool GetInterface(T** out, long type) const { if (!HasTC(type)) return false; *out = (T*)Find(type); return true; }
};
struct TPtr {
    TThing* item;
    TPtr() : item(0) { }
    TThing* GetPItem() const { return item; }
    void SetPItem(TThing* t) { item = t; }
};
inline bool Alive(const TPtr& p) { return p.GetPItem() != 0 && !(p.GetPItem()->flags91 & 1); }
inline bool AliveT(const TThing* t) { return t != 0 && !(t->flags91 & 1); }
#define CHECK(c, addr) do { if (!(c)) { std::printf("%s_TEST FAIL\n", addr); return 1; } } while (0)
// HawkingFromStall::IsStillValid: shop present, work hours, live target that is us or the shop's thing
struct TKeeper { void* shop; TThing* shopThing; void* GetShopTC() { return shop; } TThing* GetShopThing() { return shopThing; } };
struct TGroup { TThing* creature; TPtr PHawkTarget; bool workHours;
    bool IsStillValid() {
        TKeeper* k = 0; creature->GetInterface(&k, 0x90);
        if (k->GetShopTC() != 0 && workHours && Alive(PHawkTarget)) {
            TThing* self = creature;
            if (PHawkTarget.GetPItem() == self) return true;
            TThing* st = k->GetShopThing();
            if (st == PHawkTarget.GetPItem()) return true;
        }
        return false; } };
int main() {
    TThing me, stall, other; int shop = 1; TKeeper k; k.shop = &shop; k.shopThing = &stall; me.Add(0x90, &k);
    TGroup g; g.creature = &me; g.workHours = true;
    g.PHawkTarget.SetPItem(&me); CHECK(g.IsStillValid(), "0094bdf0");
    g.PHawkTarget.SetPItem(&stall); CHECK(g.IsStillValid(), "0094bdf0");
    g.PHawkTarget.SetPItem(&other); CHECK(!g.IsStillValid(), "0094bdf0");
    g.PHawkTarget.SetPItem(&me); g.workHours = false; CHECK(!g.IsStillValid(), "0094bdf0");
    std::printf("0094bdf0_TEST PASS\n"); return 0; }