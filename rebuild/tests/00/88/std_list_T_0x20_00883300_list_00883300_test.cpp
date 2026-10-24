#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x20_00883300 { void* head; unsigned long size; list_T_0x20_00883300() : head(0), size(0) { head = std::malloc(0x28); } ~list_T_0x20_00883300() { std::free(head); } }; }
int main() { std::list_T_0x20_00883300 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00883300_TEST PASS\n" : "00883300_TEST FAIL\n"); return ok ? 0 : 1; }
