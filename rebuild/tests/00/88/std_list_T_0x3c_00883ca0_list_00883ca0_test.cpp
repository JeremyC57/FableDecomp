#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x3c_00883ca0 { void* head; unsigned long size; list_T_0x3c_00883ca0() : head(0), size(0) { head = std::malloc(0x44); } ~list_T_0x3c_00883ca0() { std::free(head); } }; }
int main() { std::list_T_0x3c_00883ca0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00883ca0_TEST PASS\n" : "00883ca0_TEST FAIL\n"); return ok ? 0 : 1; }
