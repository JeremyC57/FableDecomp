#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0xc_00a7f450 { void* head; unsigned long size; list_T_0xc_00a7f450() : head(0), size(0) { head = std::malloc(0x14); } ~list_T_0xc_00a7f450() { std::free(head); } }; }
int main() { std::list_T_0xc_00a7f450 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00a7f450_TEST PASS\n" : "00a7f450_TEST FAIL\n"); return ok ? 0 : 1; }
