// Behaviour test for 00940180 (see src/00940180.cpp): VillageTaskPickCrops::IsRunnable: in hours, the village offers a crop field; plain C++98, member access only.
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

struct TVillage { TThing* field; int calls; TThing* GetCropField(TThing*) { ++calls; return field; } };
struct TVillager { TVillage* village; };
static int g_villageLookups = 0;
TVillage* GFGetPVillageInterface(TVillager* v) { ++g_villageLookups; return v->village; }
struct TGroup { TThing* creature; bool hours; TPtr PTaskThing;
    bool IsRunnable() {
        if (hours) {
            TVillager* v; if (creature->GetInterface(&v, 0x23)) GFGetPVillageInterface(v);
            TThing* f = GFGetPVillageInterface(v)->GetCropField(creature);
            if (f != 0) { PTaskThing.SetPItem(f); return true; }
        }
        return false; } };
int main() {
    TThing me, field; TVillage vil; vil.field = 0; vil.calls = 0; TVillager vr; vr.village = &vil; me.Add(0x23, &vr);
    TGroup g; g.creature = &me; g.hours = false;
    CHECK(!g.IsRunnable() && vil.calls == 0, "00940180");
    g.hours = true; CHECK(!g.IsRunnable() && vil.calls == 1 && g_villageLookups == 2, "00940180");
    vil.field = &field; CHECK(g.IsRunnable() && g.PTaskThing.GetPItem() == &field, "00940180");
    std::printf("00940180_TEST PASS\n"); return 0; }
