// Behaviour test for 00925f90 (see src/00925f90.cpp): WifeGiveYouGift::IsRunnable: a gift def to give and the live hero within 40 units; plain C++98, member access only.
#include <cstdio>
#include <cstring>
// shared test model: a thing with an interface table (sorted type -> component), a dead bit, and an
// intelligent pointer; mirrors the HasTC / GetInterface / FindEntry shapes of the sources.
struct TThing {
    unsigned long bits[9];
    long types[8];
    void* comps[8];
    int count;
    unsigned char flags91;
    unsigned long flags6c;
    TThing() : count(0), flags91(0), flags6c(0) { for (int i = 0; i < 9; ++i) bits[i] = 0; }
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
#define CHECK(c, addr) do { if (!(c)) { std::printf("%s_TEST FAIL\n", addr); return 1; } } while (0)

struct TSpouse { long gift; };
struct TGroup { TThing* creature; float pos[3]; TThing* hero; float heroPos[3]; long GiftToGiveDef;
    bool IsRunnable() {
        TSpouse* sp;
        if (creature->GetInterface(&sp, 0xD7)) {
            GiftToGiveDef = sp->gift;
            if (GiftToGiveDef > 0 && hero != 0 && !(hero->flags91 & 1)) {
                float dx = heroPos[0] - pos[0], dy = heroPos[1] - pos[1], dz = heroPos[2] - pos[2];
                float s = dz * dz; s += dx * dx; s += dy * dy;
                if (s < 1600.0f) return true;
            }
        }
        return false; } };
int main() {
    TThing me, hero; TSpouse sp; sp.gift = 0;
    TGroup g; g.creature = &me; g.hero = &hero; g.GiftToGiveDef = -1;
    for (int i = 0; i < 3; ++i) { g.pos[i] = 0; g.heroPos[i] = 0; }
    CHECK(!g.IsRunnable() && g.GiftToGiveDef == -1, "00925f90");
    me.Add(0xD7, &sp); CHECK(!g.IsRunnable() && g.GiftToGiveDef == 0, "00925f90");
    sp.gift = 12; g.heroPos[1] = 39.0f; CHECK(g.IsRunnable() && g.GiftToGiveDef == 12, "00925f90");
    g.heroPos[1] = 41.0f; CHECK(!g.IsRunnable(), "00925f90");
    g.heroPos[1] = 1.0f; hero.flags91 = 1; CHECK(!g.IsRunnable(), "00925f90");
    std::printf("00925f90_TEST PASS\n"); return 0; }
