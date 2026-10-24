#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0xc_00cdb510 { void* head; unsigned long size; list_T_0xc_00cdb510() : head(0), size(0) { head = std::malloc(0x14); } ~list_T_0xc_00cdb510() { std::free(head); } }; }
int main() { std::list_T_0xc_00cdb510 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00cdb510_TEST PASS\n" : "00cdb510_TEST FAIL\n"); return ok ? 0 : 1; }
