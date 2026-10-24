#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x60_00883be0 { void* head; unsigned long size; list_T_0x60_00883be0() : head(0), size(0) { head = std::malloc(0x68); } ~list_T_0x60_00883be0() { std::free(head); } }; }
int main() { std::list_T_0x60_00883be0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00883be0_TEST PASS\n" : "00883be0_TEST FAIL\n"); return ok ? 0 : 1; }
