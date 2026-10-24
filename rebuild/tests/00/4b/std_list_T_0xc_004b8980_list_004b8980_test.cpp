#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0xc_004b8980 { void* head; unsigned long size; list_T_0xc_004b8980() : head(0), size(0) { head = std::malloc(0x14); } ~list_T_0xc_004b8980() { std::free(head); } }; }
int main() { std::list_T_0xc_004b8980 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "004b8980_TEST PASS\n" : "004b8980_TEST FAIL\n"); return ok ? 0 : 1; }
