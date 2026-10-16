// Behaviour test for 009575c0 (see src/009575c0.cpp); plain C++98, member access only.
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
// GreetCreature::IsRunnable: interface 8 offers a creature to greet (slot 32 with true) that is visible
struct TSrc { TThing* other; bool lastFlag; TThing* GetCreatureToGreet(bool f) { lastFlag = f; return other; } };
static bool canSee = true;
static bool CanSee(TThing*, TThing*, bool) { return canSee; }
struct TGroup { TThing* creature; TPtr PCreatureToGreet;
    bool IsRunnable() {
        if (creature->HasTC(8)) {
            TThing* o = ((TSrc*)creature->Find(8))->GetCreatureToGreet(true);
            if (o != 0 && CanSee(creature, o, false)) { PCreatureToGreet.SetPItem(o); return true; }
        }
        return false; } };
int main() {
    TThing me, chicken; TSrc s; s.other = &chicken; s.lastFlag = false; TGroup g; g.creature = &me;
    CHECK(!g.IsRunnable(), "009575c0");
    me.Add(8, &s); CHECK(g.IsRunnable() && s.lastFlag && g.PCreatureToGreet.GetPItem() == &chicken, "009575c0");
    canSee = false; CHECK(!g.IsRunnable(), "009575c0");
    std::printf("009575c0_TEST PASS\n"); return 0; }
