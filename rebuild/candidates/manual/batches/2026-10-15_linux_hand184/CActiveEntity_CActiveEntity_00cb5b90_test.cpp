// Behaviour test for 00cb5b90 (see src/00cb5b90.cpp): ~CActiveEntity: every listed morph removed from the manager, the list cleared, storage freed; plain C++98, member access only.
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

struct TManager { int removed; char last[32]; TManager() : removed(0) { last[0] = 0; } void RemoveMorph(const char* n) { ++removed; std::strcpy(last, n); } };
static TManager* g_mgr = 0; static int g_created = 0;
static TManager* GetManager() { if (!g_mgr) { g_mgr = new TManager; ++g_created; } return g_mgr; }
struct TEntity { const char* names[4]; int n; int cleared;
    ~TEntity() { for (int i = 0; i < n; ++i) GetManager()->RemoveMorph(names[i]); n = 0; ++cleared; } };
int main() {
    { TEntity e; e.cleared = 0; e.n = 0; }
    CHECK(g_created == 0, "00cb5b90");
    { TEntity e; e.cleared = 0; e.names[0] = "SPARK"; e.names[1] = "GLOW"; e.n = 2; }
    CHECK(g_created == 1 && g_mgr->removed == 2 && std::strcmp(g_mgr->last, "GLOW") == 0, "00cb5b90");
    { TEntity e; e.cleared = 0; e.names[0] = "A"; e.n = 1; }
    CHECK(g_created == 1 && g_mgr->removed == 3, "00cb5b90");
    delete g_mgr;
    std::printf("00cb5b90_TEST PASS\n"); return 0; }
