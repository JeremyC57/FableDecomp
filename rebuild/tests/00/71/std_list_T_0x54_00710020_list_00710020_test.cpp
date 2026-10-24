#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x54_00710020 { void* head; unsigned long size; list_T_0x54_00710020() : head(0), size(0) { head = std::malloc(0x5c); } ~list_T_0x54_00710020() { std::free(head); } }; }
int main() { std::list_T_0x54_00710020 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00710020_TEST PASS\n" : "00710020_TEST FAIL\n"); return ok ? 0 : 1; }
