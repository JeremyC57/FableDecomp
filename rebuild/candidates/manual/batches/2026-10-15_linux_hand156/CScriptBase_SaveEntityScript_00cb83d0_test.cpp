// Behaviour test for 0x00CB83D0: CScriptBase::SaveEntityScript: the entity script persists into a fresh string; a non-empty result is stored for the entity
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
struct Script { const char* data; void Persist(char* out) { std::strcpy(out, data); } };
static char g_buf[4][64]; static int g_used = 0;
static void SaveEntityScript(PersistMap& m, unsigned long long id, Script& s)
{
    char* text = g_buf[g_used++]; text[0] = 0;
    s.Persist(text);
    if (std::strcmp(text, "") != 0) m.Set(id, text);
}
static void Run()
{
    PersistMap m; m.n = 0;
    Script a = { "A=1;" }, empty = { "" };
    SaveEntityScript(m, 7, a); CHECK(m.n == 1 && std::strcmp(m.Get(7), "A=1;") == 0);
    SaveEntityScript(m, 8, empty); CHECK(m.n == 1 && m.Get(8) == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00cb83d0_TEST FAIL\n"); return 1; }
    std::printf("00cb83d0_TEST PASS\n");
    return 0;
}
