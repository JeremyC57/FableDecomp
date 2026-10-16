// Behaviour test for 00938960 (see src/00938960.cpp): UsesSeat::IsStillValid: seated within sqrt(0.1) of the seat and not in mode 1 or 6; plain C++98, member access only.
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

struct TModes { long mode; bool HasMode(long m) { return mode == m; } };
struct TGroup { TThing* creature; float pos[3]; bool seated; float seat[3];
    bool IsStillValid() {
        if (seated) {
            float dx = seat[0] - pos[0], dy = seat[1] - pos[1], dz = seat[2] - pos[2];
            float s = dz * dz; s += dy * dy; s += dx * dx;
            if (s > 0.1f) return false;
        }
        TModes* m; creature->GetInterface(&m, 0x42);
        if (m->HasMode(1) || m->HasMode(6)) return false;
        return true; } };
int main() {
    TThing me; TModes md; md.mode = 0; me.Add(0x42, &md);
    TGroup g; g.creature = &me; g.seated = false; g.pos[0] = g.pos[1] = g.pos[2] = 0; g.seat[0] = 5; g.seat[1] = g.seat[2] = 0;
    CHECK(g.IsStillValid(), "00938960");
    g.seated = true; CHECK(!g.IsStillValid(), "00938960");
    g.seat[0] = 0.2f; CHECK(g.IsStillValid(), "00938960");
    md.mode = 6; CHECK(!g.IsStillValid(), "00938960");
    md.mode = 1; g.seated = false; CHECK(!g.IsStillValid(), "00938960");
    std::printf("00938960_TEST PASS\n"); return 0; }
