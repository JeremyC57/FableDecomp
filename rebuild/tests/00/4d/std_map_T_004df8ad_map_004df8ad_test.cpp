#include <cstdio>
static int g_calls = 0; static char* g_a = 0; static char* g_b = 0;
namespace std { struct map_T_004df8ad { void _Rb_tree_ctor(char* a, char* b) { ++g_calls; g_a = a; g_b = b; } map_T_004df8ad* map() { char* p; char* q; { char a; p = &a; } { char b; q = &b; } _Rb_tree_ctor(p, q); return (map_T_004df8ad*)this; } }; }
int main() { std::map_T_004df8ad m; bool ok = m.map() == &m && g_calls == 1 && g_a && g_b; std::printf(ok ? "004df8ad_TEST PASS\n" : "004df8ad_TEST FAIL\n"); return ok ? 0 : 1; }
