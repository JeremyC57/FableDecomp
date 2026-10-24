#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x8_00b2ec30 { void* head; unsigned long size; list_T_0x8_00b2ec30() : head(0), size(0) { head = std::malloc(0x10); } ~list_T_0x8_00b2ec30() { std::free(head); } }; }
int main() { std::list_T_0x8_00b2ec30 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00b2ec30_TEST PASS\n" : "00b2ec30_TEST FAIL\n"); return ok ? 0 : 1; }
