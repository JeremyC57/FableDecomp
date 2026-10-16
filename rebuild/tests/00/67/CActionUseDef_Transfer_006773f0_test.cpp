// Behaviour test for 006773f0 (see src/006773f0.cpp): CActionUseDef::Transfer: four fields loaded (mode 2) or saved (mode 3) in order; plain C++98, member access only.
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

struct TCtx { long mode; long in[4]; int pos; long out[4]; int wpos; int crcs;
    void CheckCRC(const char*) { ++crcs; }
    template<class T> void Transfer(T& v) { CheckCRC(""); if (mode == 2) v = (T)in[pos++]; else if (mode == 3) out[wpos++] = (long)v; } };
struct TDef { long AnimationGroup; bool Snap; bool Ignore; long Tutorial;
    void Transfer(TCtx& c) { c.Transfer(AnimationGroup); c.Transfer(Snap); c.Transfer(Ignore); c.Transfer(Tutorial); } };
int main() {
    TCtx c; c.mode = 2; c.in[0] = 11; c.in[1] = 1; c.in[2] = 0; c.in[3] = 6; c.pos = 0; c.wpos = 0; c.crcs = 0;
    TDef d; d.Transfer(c); CHECK(d.AnimationGroup == 11 && d.Snap && !d.Ignore && d.Tutorial == 6 && c.crcs == 4, "006773f0");
    c.mode = 3; d.Transfer(c); CHECK(c.out[0] == 11 && c.out[1] == 1 && c.out[2] == 0 && c.out[3] == 6 && c.wpos == 4, "006773f0");
    c.mode = 1; d.Transfer(c); CHECK(c.crcs == 12 && c.wpos == 4, "006773f0");
    std::printf("006773f0_TEST PASS\n"); return 0; }