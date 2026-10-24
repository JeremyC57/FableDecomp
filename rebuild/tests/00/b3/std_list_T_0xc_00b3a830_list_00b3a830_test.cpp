#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0xc_00b3a830 { void* head; unsigned long size; list_T_0xc_00b3a830() : head(0), size(0) { head = std::malloc(0x14); } ~list_T_0xc_00b3a830() { std::free(head); } }; }
int main() { std::list_T_0xc_00b3a830 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00b3a830_TEST PASS\n" : "00b3a830_TEST FAIL\n"); return ok ? 0 : 1; }
