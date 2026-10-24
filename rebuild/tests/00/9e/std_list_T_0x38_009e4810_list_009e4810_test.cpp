#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x38_009e4810 { void* head; unsigned long size; list_T_0x38_009e4810() : head(0), size(0) { head = std::malloc(0x40); } ~list_T_0x38_009e4810() { std::free(head); } }; }
int main() { std::list_T_0x38_009e4810 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "009e4810_TEST PASS\n" : "009e4810_TEST FAIL\n"); return ok ? 0 : 1; }
