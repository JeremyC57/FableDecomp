// Behaviour test for 00957f20 (see src/00957f20.cpp); plain C++98, member access only.
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
// ReactToBurglary::IsRunnable: sleep hours, home set, live visible player inside the home -> remember the player
static bool inRange = true, canSee = true, inside = true;
struct TGroup { TThing* creature; TPtr PHome; TThing* player; TPtr PBurglar;
    bool IsRunnable() {
        bool sleepTime = inRange; TThing* home = PHome.GetPItem();
        if (sleepTime && home != 0) {
            TThing* p = player;
            if (p != 0 && !(p->flags91 & 1) && canSee && inside) { PBurglar.SetPItem(p); return true; }
        }
        return false; } };
int main() {
    TThing me, house, hero; TGroup g; g.creature = &me; g.player = &hero;
    CHECK(!g.IsRunnable(), "00957f20");
    g.PHome.SetPItem(&house); CHECK(g.IsRunnable() && g.PBurglar.GetPItem() == &hero, "00957f20");
    inside = false; CHECK(!g.IsRunnable(), "00957f20");
    inside = true; inRange = false; CHECK(!g.IsRunnable(), "00957f20");
    std::printf("00957f20_TEST PASS\n"); return 0; }
