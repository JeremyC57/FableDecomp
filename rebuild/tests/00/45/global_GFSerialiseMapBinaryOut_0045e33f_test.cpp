// behaviour test for 0x0045E33F: the writer emits the count then key/flag for each entry in order
#include <cstdio>
#include <cstring>
static char g_log[256]; static int g_n = 0;
struct Out { void WriteSLONG(long v) { g_n += std::sprintf(g_log + g_n, "%ld;", v); } void WriteEBOOL(bool b) { g_n += std::sprintf(g_log + g_n, "%c;", b ? 'T' : 'F'); } };
struct E { long k; bool v; };
static void Serialise(Out& out, const E* e, int n) { out.WriteSLONG(n); for (int i = 0; i < n; ++i) { out.WriteSLONG(e[i].k); out.WriteEBOOL(e[i].v); } }
int main()
{
    E e[2] = { { 3, true }, { 9, false } }; Out out; Serialise(out, e, 2);
    if (std::strcmp(g_log, "2;3;T;9;F;") != 0) { std::printf("0045e33f_TEST FAIL\n"); return 1; }
    std::printf("0045e33f_TEST PASS\n");
    return 0;
}