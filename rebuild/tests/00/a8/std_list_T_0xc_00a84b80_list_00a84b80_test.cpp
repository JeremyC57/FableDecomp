#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0xc_00a84b80 { void* head; unsigned long size; list_T_0xc_00a84b80() : head(0), size(0) { head = std::malloc(0x14); } ~list_T_0xc_00a84b80() { std::free(head); } }; }
int main() { std::list_T_0xc_00a84b80 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00a84b80_TEST PASS\n" : "00a84b80_TEST FAIL\n"); return ok ? 0 : 1; }
