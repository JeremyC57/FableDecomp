// Behaviour test for 009574d0 (see src/009574d0.cpp); plain C++98, member access only.
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
// GreetCreature::Update: alive greet target + facing -> play SG_CHICKEN_WANDER and wait; finish exactly once
#include <cstring>
static int lookResult = 2, performResult = 2, finished = 0, played = 0;
struct TGroup { TPtr PCreatureToGreet;
    void FinishUpdate() { ++finished; }
    void Update() {
        if (Alive(PCreatureToGreet) && lookResult == 2) {
            const char* animation = "SG_CHICKEN_WANDER";
            ++played; long result = performResult; (void)animation;
            if (result != 2) { FinishUpdate(); return; }
        }
        FinishUpdate(); } };
int main() {
    TThing chick; TGroup g; g.PCreatureToGreet.SetPItem(&chick);
    g.Update(); CHECK(played == 1 && finished == 1, "009574d0");
    performResult = 1; g.Update(); CHECK(played == 2 && finished == 2, "009574d0");
    chick.flags91 = 1; g.Update(); CHECK(played == 2 && finished == 3, "009574d0");
    std::printf("009574d0_TEST PASS\n"); return 0; }
