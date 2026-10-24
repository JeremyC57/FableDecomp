#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x18_0070a7d0 { void* head; unsigned long size; list_T_0x18_0070a7d0() : head(0), size(0) { head = std::malloc(0x20); } ~list_T_0x18_0070a7d0() { std::free(head); } }; }
int main() { std::list_T_0x18_0070a7d0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "0070a7d0_TEST PASS\n" : "0070a7d0_TEST FAIL\n"); return ok ? 0 : 1; }
