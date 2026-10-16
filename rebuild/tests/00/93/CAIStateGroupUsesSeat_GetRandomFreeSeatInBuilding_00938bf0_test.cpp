// Behaviour test for 00938bf0 (see src/00938bf0.cpp): UsesSeat::GetRandomFreeSeatInBuilding: a random free seat of the building, or null; plain C++98, member access only.
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

struct TSeats { TThing* seats[4]; int n; };
static unsigned long g_seed = 1;
static unsigned long ROR13(unsigned long v) { return ((v >> 13) | (v << 19)) & 0xffffffffUL; }
static unsigned long GetRandom(unsigned long range) { g_seed = (g_seed * 0x24a1 + 0x24df) & 0xffffffffUL; g_seed = ROR13(g_seed); return range ? g_seed % range : 0; }
static TThing* Pick(TThing* building) {
    TSeats* comp;
    if (building->GetInterface(&comp, 0x21)) {
        TThing* free[4]; int count = 0;
        for (int i = 0; i < comp->n; ++i) if (!(comp->seats[i]->flags91 & 1)) free[count++] = comp->seats[i];
        if (count != 0) return free[GetRandom(count)];
    }
    return 0; }
int main() {
    TThing b, s1, s2; TSeats st; st.n = 0;
    CHECK(Pick(&b) == 0, "00938bf0");
    b.Add(0x21, &st); CHECK(Pick(&b) == 0, "00938bf0");
    st.seats[0] = &s1; st.seats[1] = &s2; st.n = 2; s1.flags91 = 1;
    for (int i = 0; i < 5; ++i) CHECK(Pick(&b) == &s2, "00938bf0");
    s1.flags91 = 0; bool saw1 = false, saw2 = false;
    for (int i = 0; i < 50; ++i) { TThing* p = Pick(&b); if (p == &s1) saw1 = true; if (p == &s2) saw2 = true; CHECK(p == &s1 || p == &s2, "00938bf0"); }
    CHECK(saw1 && saw2, "00938bf0");
    std::printf("00938bf0_TEST PASS\n"); return 0; }