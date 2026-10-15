// Behaviour test for 0x00CB7EE0: CScriptBase::LoadEntityScript: a persisted script text for the entity is copied and fed to the persist loader; nothing happens otherwise
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct Entry { unsigned long long id; const char* script; };
struct PersistMap {
    Entry e[4]; int n;
    Entry* LowerBound(const unsigned long long& k) { int i = 0; while (i < n && e[i].id < k) ++i; return e + i; }
    Entry* end() { return e + n; }
    Entry* find(unsigned long long key) { unsigned long long k = key; Entry* it = LowerBound(k); if (it != end() && key >= it->id) return it; return end(); }
    const char* Get(unsigned long long id) { Entry* it = find(id); return it == end() ? 0 : it->script; }
    void Set(unsigned long long id, const char* s) { Entry* it = LowerBound(id); if (it != end() && it->id == id) { it->script = s; return; } for (Entry* p = end(); p != it; --p) *p = *(p - 1); it->id = id; it->script = s; ++n; }
};
struct Active { char loaded[64]; int loads; };
static void DoPersist(Active& a, const char* text, bool saving) { if (!saving) { std::strcpy(a.loaded, text); ++a.loads; } }
static void LoadEntityScript(PersistMap& m, unsigned long long id, Active& a) { const char* p = m.Get(id); if (p) { char copy[64]; std::strcpy(copy, p); DoPersist(a, copy, false); } }
static void Run()
{
    PersistMap m; m.n = 0; m.Set(0x100000005ULL, "X=1;"); m.Set(3, "Y=2;");
    Active a; a.loads = 0; a.loaded[0] = 0;
    LoadEntityScript(m, 0x100000005ULL, a); CHECK(a.loads == 1 && std::strcmp(a.loaded, "X=1;") == 0);
    LoadEntityScript(m, 5, a); CHECK(a.loads == 1);          // same low word, different high word: not found
    LoadEntityScript(m, 3, a); CHECK(a.loads == 2 && std::strcmp(a.loaded, "Y=2;") == 0);
    LoadEntityScript(m, 99, a); CHECK(a.loads == 2);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00cb7ee0_TEST FAIL\n"); return 1; }
    std::printf("00cb7ee0_TEST PASS\n");
    return 0;
}
