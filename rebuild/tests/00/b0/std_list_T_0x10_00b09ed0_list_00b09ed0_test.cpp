#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x10_00b09ed0 { void* head; unsigned long size; list_T_0x10_00b09ed0() : head(0), size(0) { head = std::malloc(0x18); } ~list_T_0x10_00b09ed0() { std::free(head); } }; }
int main() { std::list_T_0x10_00b09ed0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00b09ed0_TEST PASS\n" : "00b09ed0_TEST FAIL\n"); return ok ? 0 : 1; }
