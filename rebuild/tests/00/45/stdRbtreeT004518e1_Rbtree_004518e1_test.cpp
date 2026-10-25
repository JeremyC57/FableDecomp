// Behaviour test for std::_Rb_tree<T_004518e1>::~_Rb_tree() (0x004518E1): clear() once, then free the header node when
// there is one.
#include <cstdio>
#include <cstdlib>
static int g_cleared = 0; static void* g_freed = 0;
static void MyFree(void* p) { g_freed = p; std::free(p); }
namespace std { struct _Rb_tree_T_004518e1 { void* _M_header; unsigned long _M_node_count; void clear() { ++g_cleared; _M_node_count = 0; } ~_Rb_tree_T_004518e1() { clear(); if (_M_header) MyFree(_M_header); } }; }
int main() { void* n = std::malloc(8); { std::_Rb_tree_T_004518e1 t; t._M_header = n; t._M_node_count = 3; } bool ok = g_cleared == 1 && g_freed == n; { std::_Rb_tree_T_004518e1 t; t._M_header = 0; t._M_node_count = 0; } ok = ok && g_cleared == 2 && g_freed == n; std::printf(ok ? "004518e1_TEST PASS\n" : "004518e1_TEST FAIL\n"); return ok ? 0 : 1; }
