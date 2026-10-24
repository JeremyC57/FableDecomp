#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x1c_0076fbf0 { void* head; unsigned long size; list_T_0x1c_0076fbf0() : head(0), size(0) { head = std::malloc(0x24); } ~list_T_0x1c_0076fbf0() { std::free(head); } }; }
int main() { std::list_T_0x1c_0076fbf0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "0076fbf0_TEST PASS\n" : "0076fbf0_TEST FAIL\n"); return ok ? 0 : 1; }
