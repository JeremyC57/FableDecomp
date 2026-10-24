#include <cstdio>
#include <cstdlib>
static int g_cleared = 0; static void* g_freed = 0;
static void MyFree(void* p) { g_freed = p; std::free(p); }
namespace std { struct list_T_005932a3 { void* _M_node; void clear() { ++g_cleared; } ~list_T_005932a3() { clear(); if (_M_node) MyFree(_M_node); } }; }
int main() { void* n = std::malloc(8); { std::list_T_005932a3 l; l._M_node = n; } bool ok = g_cleared == 1 && g_freed == n; { std::list_T_005932a3 l; l._M_node = 0; } ok = ok && g_cleared == 2 && g_freed == n; std::printf(ok ? "005932a3_TEST PASS\n" : "005932a3_TEST FAIL\n"); return ok ? 0 : 1; }
