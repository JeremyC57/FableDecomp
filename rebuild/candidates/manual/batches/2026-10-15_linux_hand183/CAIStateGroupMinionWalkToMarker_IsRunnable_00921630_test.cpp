// Behaviour test for 00921630 (see src/00921630.cpp); plain C++98, member access only.
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
// WalkToMarker::IsRunnable: mode 8 or 9 and a thing named like the marker exists
#include <cstring>
struct TWalk { long Mode; const char* MarkerName; };
static const char* world[2] = { "MARKER_A", "MARKER_B" };
static bool GetPFirstThing(const char* name) { for (int i = 0; i < 2; ++i) if (std::strcmp(world[i], name) == 0) return true; return false; }
static bool IsRunnable(TThing* creature) {
    TWalk* w = 0; creature->GetInterface(&w, 0xaa);
    if (w->Mode == 8 || w->Mode == 9) { bool found; { found = GetPFirstThing(w->MarkerName); } if (found) return true; }
    return false; }
int main() {
    TThing me; TWalk w; w.Mode = 8; w.MarkerName = "MARKER_B"; me.Add(0xaa, &w);
    CHECK(IsRunnable(&me), "00921630");
    w.MarkerName = "NOPE"; CHECK(!IsRunnable(&me), "00921630");
    w.MarkerName = "MARKER_A"; w.Mode = 7; CHECK(!IsRunnable(&me), "00921630");
    w.Mode = 9; CHECK(IsRunnable(&me), "00921630");
    std::printf("00921630_TEST PASS\n"); return 0; }
