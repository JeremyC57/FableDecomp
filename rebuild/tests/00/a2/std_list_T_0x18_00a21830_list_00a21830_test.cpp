#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x18_00a21830 { void* head; unsigned long size; list_T_0x18_00a21830() : head(0), size(0) { head = std::malloc(0x20); } ~list_T_0x18_00a21830() { std::free(head); } }; }
int main() { std::list_T_0x18_00a21830 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00a21830_TEST PASS\n" : "00a21830_TEST FAIL\n"); return ok ? 0 : 1; }
