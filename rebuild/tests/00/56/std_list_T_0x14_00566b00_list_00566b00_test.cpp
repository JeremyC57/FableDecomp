#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x14_00566b00 { void* head; unsigned long size; list_T_0x14_00566b00() : head(0), size(0) { head = std::malloc(0x1c); } ~list_T_0x14_00566b00() { std::free(head); } }; }
int main() { std::list_T_0x14_00566b00 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00566b00_TEST PASS\n" : "00566b00_TEST FAIL\n"); return ok ? 0 : 1; }
