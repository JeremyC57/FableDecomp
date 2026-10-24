#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0xc_00a214a0 { void* head; unsigned long size; list_T_0xc_00a214a0() : head(0), size(0) { head = std::malloc(0x14); } ~list_T_0xc_00a214a0() { std::free(head); } }; }
int main() { std::list_T_0xc_00a214a0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00a214a0_TEST PASS\n" : "00a214a0_TEST FAIL\n"); return ok ? 0 : 1; }
