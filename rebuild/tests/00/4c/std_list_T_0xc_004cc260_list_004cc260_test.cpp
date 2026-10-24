#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0xc_004cc260 { void* head; unsigned long size; list_T_0xc_004cc260() : head(0), size(0) { head = std::malloc(0x14); } ~list_T_0xc_004cc260() { std::free(head); } }; }
int main() { std::list_T_0xc_004cc260 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "004cc260_TEST PASS\n" : "004cc260_TEST FAIL\n"); return ok ? 0 : 1; }
