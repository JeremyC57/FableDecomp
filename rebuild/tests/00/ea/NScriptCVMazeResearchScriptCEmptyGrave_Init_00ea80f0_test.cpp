#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
struct FlagMap { const char* Keys[4]; bool Values[4]; int Count;
    bool& operator[](const char* k) { for (int i = 0; i < Count; ++i) if (std::strcmp(Keys[i], k) == 0) return Values[i]; Keys[Count] = k; Values[Count] = false; return Values[Count++]; } };
struct Parent { FlagMap cs_flags; };
struct E { Parent* ParentClass; void Init() { ParentClass->cs_flags["UNLIMBO"] = false; } };
int main()
{
    g_log[0] = 0;
    bool ok = true;
    Parent p; p.cs_flags.Count = 1; p.cs_flags.Keys[0] = "UNLIMBO"; p.cs_flags.Values[0] = true;
    E e; e.ParentClass = &p; e.Init();
    ok = p.cs_flags.Count == 1 && !p.cs_flags.Values[0];
    Parent q; q.cs_flags.Count = 0; e.ParentClass = &q; e.Init();
    ok = ok && q.cs_flags.Count == 1 && !q.cs_flags.Values[0];
    if (!ok) { std::printf("00ea80f0_TEST FAIL\n"); return 1; }
    std::printf("00ea80f0_TEST PASS\n");
    return 0;
}