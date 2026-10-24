#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0xc_00cdb710 { void* head; unsigned long size; list_T_0xc_00cdb710() : head(0), size(0) { head = std::malloc(0x14); } ~list_T_0xc_00cdb710() { std::free(head); } }; }
int main() { std::list_T_0xc_00cdb710 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00cdb710_TEST PASS\n" : "00cdb710_TEST FAIL\n"); return ok ? 0 : 1; }
