#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x1c_009be170 { void* head; unsigned long size; list_T_0x1c_009be170() : head(0), size(0) { head = std::malloc(0x24); } ~list_T_0x1c_009be170() { std::free(head); } }; }
int main() { std::list_T_0x1c_009be170 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "009be170_TEST PASS\n" : "009be170_TEST FAIL\n"); return ok ? 0 : 1; }
