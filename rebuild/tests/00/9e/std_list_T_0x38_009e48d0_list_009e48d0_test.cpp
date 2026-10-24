#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x38_009e48d0 { void* head; unsigned long size; list_T_0x38_009e48d0() : head(0), size(0) { head = std::malloc(0x40); } ~list_T_0x38_009e48d0() { std::free(head); } }; }
int main() { std::list_T_0x38_009e48d0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "009e48d0_TEST PASS\n" : "009e48d0_TEST FAIL\n"); return ok ? 0 : 1; }
