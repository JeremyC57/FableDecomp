#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x10_004ba480 { void* head; unsigned long size; list_T_0x10_004ba480() : head(0), size(0) { head = std::malloc(0x18); } ~list_T_0x10_004ba480() { std::free(head); } }; }
int main() { std::list_T_0x10_004ba480 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "004ba480_TEST PASS\n" : "004ba480_TEST FAIL\n"); return ok ? 0 : 1; }
