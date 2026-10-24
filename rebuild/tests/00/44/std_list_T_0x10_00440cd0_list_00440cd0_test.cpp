#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x10_00440cd0 { void* head; unsigned long size; list_T_0x10_00440cd0() : head(0), size(0) { head = std::malloc(0x18); } ~list_T_0x10_00440cd0() { std::free(head); } }; }
int main() { std::list_T_0x10_00440cd0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00440cd0_TEST PASS\n" : "00440cd0_TEST FAIL\n"); return ok ? 0 : 1; }
