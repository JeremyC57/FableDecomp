// behaviour test for 0x0046367A: the writer emits the count then key/value for each entry in order
#include <cstdio>
#include <cstring>
static char g_log[256]; static int g_n = 0;
struct Out { void WriteSLONG(long v) { g_n += std::sprintf(g_log + g_n, "%ld;", v); } void WriteString(const char* s) { g_n += std::sprintf(g_log + g_n, "%s;", s); } };
struct E { const char* k; long v; };
static void Serialise(Out& out, const E* e, int n) { out.WriteSLONG(n); for (int i = 0; i < n; ++i) { out.WriteString(e[i].k); out.WriteSLONG(e[i].v); } }
int main()
{
    E e[2] = { { "a", 3 }, { "b", 7 } }; Out out; Serialise(out, e, 2);
    if (std::strcmp(g_log, "2;a;3;b;7;") != 0) { std::printf("0046367a_TEST FAIL\n"); return 1; }
    std::printf("0046367a_TEST PASS\n");
    return 0;
}