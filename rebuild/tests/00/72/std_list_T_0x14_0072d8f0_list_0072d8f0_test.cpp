#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x14_0072d8f0 { void* head; unsigned long size; list_T_0x14_0072d8f0() : head(0), size(0) { head = std::malloc(0x1c); } ~list_T_0x14_0072d8f0() { std::free(head); } }; }
int main() { std::list_T_0x14_0072d8f0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "0072d8f0_TEST PASS\n" : "0072d8f0_TEST FAIL\n"); return ok ? 0 : 1; }
