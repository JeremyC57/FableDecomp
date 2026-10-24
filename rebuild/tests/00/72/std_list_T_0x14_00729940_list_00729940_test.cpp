#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x14_00729940 { void* head; unsigned long size; list_T_0x14_00729940() : head(0), size(0) { head = std::malloc(0x1c); } ~list_T_0x14_00729940() { std::free(head); } }; }
int main() { std::list_T_0x14_00729940 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00729940_TEST PASS\n" : "00729940_TEST FAIL\n"); return ok ? 0 : 1; }
