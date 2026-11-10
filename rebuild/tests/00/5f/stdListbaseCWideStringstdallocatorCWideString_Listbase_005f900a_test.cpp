#include <cstdio>
#include <cstdlib>
static int g_cleared = 0; static void* g_freed = 0;
static void MyFree(void* p) { g_freed = p; std::free(p); }
struct ListBase { void* _M_node; void clear() { ++g_cleared; } ~ListBase() { clear(); if (_M_node) MyFree(_M_node); } };   // _List_base<CWideString>
int main() { void* n = std::malloc(8); { ListBase l; l._M_node = n; } bool ok = g_cleared == 1 && g_freed == n; { ListBase l; l._M_node = 0; } ok = ok && g_cleared == 2 && g_freed == n; std::printf(ok ? "005f900a_TEST PASS\n" : "005f900a_TEST FAIL\n"); return ok ? 0 : 1; }
