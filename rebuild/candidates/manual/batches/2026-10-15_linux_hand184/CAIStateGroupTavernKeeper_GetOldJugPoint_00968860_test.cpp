// Behaviour test for 00968860 (see src/00968860.cpp): TavernKeeper::GetOldJugPoint: "ACTION_POINT_BARMAN_0<JugSlot+1>" from the live bar; plain C++98, member access only.
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

struct TPoints { char lastName[64]; long lastSlot; bool result;
    bool GetFreeNamedActionPoint(TThing*, float*, float*, const char* name, long* ap, long slot, bool) { std::strcpy(lastName, name); lastSlot = slot; *ap = 5; return result; } };
static void BuildName(char* out, const char* base, long n) { std::sprintf(out, "%s_0%ld", base, n); }
struct TGroup { TThing* creature; TPtr PBar; long Index; long ApIndex;
    bool GetOldJugPoint(const char* base) {
        if (!Alive(PBar)) return false;
        TPoints* p = 0; PBar.GetPItem()->GetInterface(&p, 0x2E);
        char name[64]; BuildName(name, base, Index + 1);
        return p->GetFreeNamedActionPoint(creature, 0, 0, name, &ApIndex, 0, true); } };
int main() {
    TThing me, bar; TPoints pts; pts.result = true; pts.lastSlot = -1; bar.Add(0x2E, &pts);
    TGroup g; g.creature = &me; g.Index = 2; g.ApIndex = 0;
    CHECK(!g.GetOldJugPoint("ACTION_POINT_BARMAN"), "00968860");
    g.PBar.SetPItem(&bar);
    CHECK(g.GetOldJugPoint("ACTION_POINT_BARMAN") && std::strcmp(pts.lastName, "ACTION_POINT_BARMAN_03") == 0 && pts.lastSlot == 0 && g.ApIndex == 5, "00968860");
    pts.result = false; CHECK(!g.GetOldJugPoint("ACTION_POINT_BARMAN"), "00968860");
    bar.flags91 = 1; pts.result = true; CHECK(!g.GetOldJugPoint("ACTION_POINT_BARMAN"), "00968860");
    std::printf("00968860_TEST PASS\n"); return 0; }
