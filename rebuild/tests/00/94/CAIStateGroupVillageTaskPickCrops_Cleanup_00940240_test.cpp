// Behaviour test for 00940240 (see src/00940240.cpp): VillageTaskPickCrops::Cleanup: hand the task thing back to the village and clear it; plain C++98, member access only.
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

struct TVillage { TThing* task; TThing* who; int calls; void RemoveThingFromTask(TThing* t, TThing* c) { task = t; who = c; ++calls; } };
struct TVillager { TVillage* village; };
TVillage* GFGetPVillageInterface(TVillager* v) { return v->village; }
struct TGroup { TThing* creature; TPtr PTaskThing;
    void Cleanup() {
        if (PTaskThing.GetPItem() != 0) {
            bool hasVillager = creature->HasTC(0x23) != 0;
            if (hasVillager) { TVillage* v = GFGetPVillageInterface((TVillager*)creature->Find(0x23)); TThing* c = creature; v->RemoveThingFromTask(PTaskThing.GetPItem(), c); }
            if (hasVillager) PTaskThing.SetPItem(0);
        } } };
int main() {
    TThing me, field; TVillage vil; vil.task = 0; vil.who = 0; vil.calls = 0; TVillager vr; vr.village = &vil;
    TGroup g; g.creature = &me; g.Cleanup(); CHECK(vil.calls == 0, "00940240");
    g.PTaskThing.SetPItem(&field); g.Cleanup(); CHECK(vil.calls == 0 && g.PTaskThing.GetPItem() == &field, "00940240");
    me.Add(0x23, &vr); g.Cleanup(); CHECK(vil.calls == 1 && vil.task == &field && vil.who == &me && g.PTaskThing.GetPItem() == 0, "00940240");
    std::printf("00940240_TEST PASS\n"); return 0; }