#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0xc_00a0a460 { void* head; unsigned long size; list_T_0xc_00a0a460() : head(0), size(0) { head = std::malloc(0x14); } ~list_T_0xc_00a0a460() { std::free(head); } }; }
int main() { std::list_T_0xc_00a0a460 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00a0a460_TEST PASS\n" : "00a0a460_TEST FAIL\n"); return ok ? 0 : 1; }
