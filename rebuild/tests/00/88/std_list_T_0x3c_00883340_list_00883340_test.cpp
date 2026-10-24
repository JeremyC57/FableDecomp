#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x3c_00883340 { void* head; unsigned long size; list_T_0x3c_00883340() : head(0), size(0) { head = std::malloc(0x44); } ~list_T_0x3c_00883340() { std::free(head); } }; }
int main() { std::list_T_0x3c_00883340 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00883340_TEST PASS\n" : "00883340_TEST FAIL\n"); return ok ? 0 : 1; }
