#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x18_0043fc20 { void* head; unsigned long size; list_T_0x18_0043fc20() : head(0), size(0) { head = std::malloc(0x20); } ~list_T_0x18_0043fc20() { std::free(head); } }; }
int main() { std::list_T_0x18_0043fc20 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "0043fc20_TEST PASS\n" : "0043fc20_TEST FAIL\n"); return ok ? 0 : 1; }
