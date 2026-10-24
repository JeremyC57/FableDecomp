#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x60_00883280 { void* head; unsigned long size; list_T_0x60_00883280() : head(0), size(0) { head = std::malloc(0x68); } ~list_T_0x60_00883280() { std::free(head); } }; }
int main() { std::list_T_0x60_00883280 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00883280_TEST PASS\n" : "00883280_TEST FAIL\n"); return ok ? 0 : 1; }
