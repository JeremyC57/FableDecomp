// Behaviour test for 0091c130 (see src/0091c130.cpp); plain C++98, member access only.
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
// NymphFloatAround::OnReplace: kill + clear the live float target, stop the nymph flying around
struct TKillable { int killed; };
struct TNymph { long stopped; void StopFlyingAround(long m) { stopped = m; } };
static int kills = 0;
struct TGroup { TThing* creature; TPtr PFloatTarget;
    void OnReplace() {
        if (Alive(PFloatTarget)) { ++kills; PFloatTarget.SetPItem(0); }
        TNymph* n = 0; if (creature->GetInterface(&n, 0xf5)) n->StopFlyingAround(-1); } };
int main() {
    TThing me, wisp; TNymph n; n.stopped = 0; me.Add(0xf5, &n); TGroup g; g.creature = &me; g.PFloatTarget.SetPItem(&wisp);
    g.OnReplace(); CHECK(kills == 1 && g.PFloatTarget.GetPItem() == 0 && n.stopped == -1, "0091c130");
    TThing other; TGroup h; h.creature = &other; h.OnReplace(); CHECK(kills == 1, "0091c130");
    std::printf("0091c130_TEST PASS\n"); return 0; }
