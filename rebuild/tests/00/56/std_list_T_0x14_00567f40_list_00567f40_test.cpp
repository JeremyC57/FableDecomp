#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x14_00567f40 { void* head; unsigned long size; list_T_0x14_00567f40() : head(0), size(0) { head = std::malloc(0x1c); } ~list_T_0x14_00567f40() { std::free(head); } }; }
int main() { std::list_T_0x14_00567f40 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00567f40_TEST PASS\n" : "00567f40_TEST FAIL\n"); return ok ? 0 : 1; }
