#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x40_008fbd60 { void* head; unsigned long size; list_T_0x40_008fbd60() : head(0), size(0) { head = std::malloc(0x48); } ~list_T_0x40_008fbd60() { std::free(head); } }; }
int main() { std::list_T_0x40_008fbd60 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "008fbd60_TEST PASS\n" : "008fbd60_TEST FAIL\n"); return ok ? 0 : 1; }
