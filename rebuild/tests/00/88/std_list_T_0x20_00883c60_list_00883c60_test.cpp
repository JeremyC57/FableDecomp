#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x20_00883c60 { void* head; unsigned long size; list_T_0x20_00883c60() : head(0), size(0) { head = std::malloc(0x28); } ~list_T_0x20_00883c60() { std::free(head); } }; }
int main() { std::list_T_0x20_00883c60 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00883c60_TEST PASS\n" : "00883c60_TEST FAIL\n"); return ok ? 0 : 1; }
