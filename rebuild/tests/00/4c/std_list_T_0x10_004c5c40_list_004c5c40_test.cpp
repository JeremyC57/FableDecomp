#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x10_004c5c40 { void* head; unsigned long size; list_T_0x10_004c5c40() : head(0), size(0) { head = std::malloc(0x18); } ~list_T_0x10_004c5c40() { std::free(head); } }; }
int main() { std::list_T_0x10_004c5c40 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "004c5c40_TEST PASS\n" : "004c5c40_TEST FAIL\n"); return ok ? 0 : 1; }
