#include <cstdio>
static int g_calls = 0; static char* g_a = 0; static char* g_b = 0;
namespace std { struct map_T_0059b32a { void _Rb_tree_ctor(char* a, char* b) { ++g_calls; g_a = a; g_b = b; } map_T_0059b32a* map() { char* p; char* q; { char a; p = &a; } { char b; q = &b; } _Rb_tree_ctor(p, q); return (map_T_0059b32a*)this; } }; }
int main() { std::map_T_0059b32a m; bool ok = m.map() == &m && g_calls == 1 && g_a && g_b; std::printf(ok ? "0059b32a_TEST PASS\n" : "0059b32a_TEST FAIL\n"); return ok ? 0 : 1; }
