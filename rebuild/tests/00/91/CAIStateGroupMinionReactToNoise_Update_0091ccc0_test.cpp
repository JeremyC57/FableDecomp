// Behaviour test for 0091ccc0 (see src/0091ccc0.cpp); plain C++98, member access only.
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
// ReactToNoise::Update: yield while busy (stop early if stopping), look at the noise, walk until not 4; clear flags on 2
static int busy = 2, stopping = 0, yields = 0, finished = 0, moves = 0; static long lookResult = 2;
static long moveResults[3] = { 4, 4, 2 };
struct TGroup { bool InvestigatingNoise, FoundNoiseMaker;
    void Update() {
        while (busy > 0) { if (stopping) { ++finished; return; } --busy; ++yields; }
        if (lookResult == 2) { long r; do { r = moveResults[moves++]; } while (r == 4);
            if (r == 2) { InvestigatingNoise = false; FoundNoiseMaker = false; } }
        ++finished; } };
int main() {
    TGroup g; g.InvestigatingNoise = true; g.FoundNoiseMaker = true; g.Update();
    CHECK(yields == 2 && moves == 3 && !g.InvestigatingNoise && !g.FoundNoiseMaker && finished == 1, "0091ccc0");
    busy = 1; stopping = 1; g.InvestigatingNoise = true; g.Update(); CHECK(finished == 2 && g.InvestigatingNoise, "0091ccc0");
    std::printf("0091ccc0_TEST PASS\n"); return 0; }