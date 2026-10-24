#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x10_004b8900 { void* head; unsigned long size; list_T_0x10_004b8900() : head(0), size(0) { head = std::malloc(0x18); } ~list_T_0x10_004b8900() { std::free(head); } }; }
int main() { std::list_T_0x10_004b8900 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "004b8900_TEST PASS\n" : "004b8900_TEST FAIL\n"); return ok ? 0 : 1; }
