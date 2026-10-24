#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0xc_007cbc40 { void* head; unsigned long size; list_T_0xc_007cbc40() : head(0), size(0) { head = std::malloc(0x14); } ~list_T_0xc_007cbc40() { std::free(head); } }; }
int main() { std::list_T_0xc_007cbc40 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "007cbc40_TEST PASS\n" : "007cbc40_TEST FAIL\n"); return ok ? 0 : 1; }
