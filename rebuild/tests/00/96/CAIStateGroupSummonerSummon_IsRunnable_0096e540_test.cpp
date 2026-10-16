// Behaviour test for 0096e540 (see src/0096e540.cpp): SummonerSummon::IsRunnable: a live interface-8 target and a summon component that allows creatures; plain C++98, member access only.
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

struct TTargetSrc { TThing* enemy; TThing* GetEnemy() { return enemy; } };
struct TSummon { long maxCount; long GetMaxCreatureCount() { return maxCount; } };
struct TGroup { TThing* creature; TPtr PTarget;
    bool IsRunnable() {
        if (creature->HasTC(8)) {
            TThing* target = ((TTargetSrc*)creature->Find(8))->GetEnemy();
            if (target != 0 && !(target->flags91 & 1) && creature->HasTC(0xFA)
                && ((TSummon*)creature->Find(0xFA))->GetMaxCreatureCount() != 0) {
                PTarget.SetPItem(target);
                return true;
            }
        }
        return false; } };
int main() {
    TThing me, foe; TTargetSrc src; src.enemy = &foe; TSummon sum; sum.maxCount = 2;
    TGroup g; g.creature = &me;
    CHECK(!g.IsRunnable(), "0096e540");
    me.Add(8, &src); CHECK(!g.IsRunnable(), "0096e540");
    me.Add(0xFA, &sum); CHECK(g.IsRunnable() && g.PTarget.GetPItem() == &foe, "0096e540");
    sum.maxCount = 0; g.PTarget.SetPItem(0); CHECK(!g.IsRunnable() && g.PTarget.GetPItem() == 0, "0096e540");
    sum.maxCount = 1; foe.flags91 = 1; CHECK(!g.IsRunnable(), "0096e540");
    src.enemy = 0; CHECK(!g.IsRunnable(), "0096e540");
    std::printf("0096e540_TEST PASS\n"); return 0; }