// Behaviour test for 00967180 (see src/00967180.cpp): TavernServer::GetJug: keep a live jug, else take the bar jug at a free jug point; plain C++98, member access only.
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

struct TBar { bool free; TThing* jug; long lastIndex;
    bool GetFreeJugPoint(TThing*, float*, float*, long* ap, long* idx, bool) { if (!free) return false; *ap = 7; *idx = 3; return true; }
    TThing* GetJug(long index) { lastIndex = index; return jug; } };
struct TGroup { TThing* creature; TPtr PJug; TPtr PBar; long JugIndex; long BarActionPointIndex; float BarPos[3]; float BarRHSet[6];
    bool GetJug() {
        if (Alive(PJug)) return true;
        TThing* bar = PBar.GetPItem();
        if (bar->HasTC(0xB9)) {
            TBar* tb = (TBar*)bar->Find(0xB9);
            TThing* c = creature;
            if (tb->GetFreeJugPoint(c, BarPos, BarRHSet, &BarActionPointIndex, &JugIndex, true)) { PJug.SetPItem(tb->GetJug(JugIndex)); return true; }
        }
        return false; } };
int main() {
    TThing me, barThing, jug; TBar tb; tb.free = false; tb.jug = &jug; tb.lastIndex = -1;
    TGroup g; g.creature = &me; g.PBar.SetPItem(&barThing); g.JugIndex = 0; g.BarActionPointIndex = 0;
    CHECK(!g.GetJug(), "00967180");
    barThing.Add(0xB9, &tb); CHECK(!g.GetJug(), "00967180");
    tb.free = true; CHECK(g.GetJug() && g.PJug.GetPItem() == &jug && tb.lastIndex == 3 && g.BarActionPointIndex == 7, "00967180");
    tb.lastIndex = -1; CHECK(g.GetJug() && tb.lastIndex == -1, "00967180");
    jug.flags91 = 1; tb.free = false; CHECK(!g.GetJug(), "00967180");
    std::printf("00967180_TEST PASS\n"); return 0; }
