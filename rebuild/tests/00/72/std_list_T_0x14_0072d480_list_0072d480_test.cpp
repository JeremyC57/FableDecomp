#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x14_0072d480 { void* head; unsigned long size; list_T_0x14_0072d480() : head(0), size(0) { head = std::malloc(0x1c); } ~list_T_0x14_0072d480() { std::free(head); } }; }
int main() { std::list_T_0x14_0072d480 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "0072d480_TEST PASS\n" : "0072d480_TEST FAIL\n"); return ok ? 0 : 1; }
