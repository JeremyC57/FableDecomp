#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x44_00758ae0 { void* head; unsigned long size; list_T_0x44_00758ae0() : head(0), size(0) { head = std::malloc(0x4c); } ~list_T_0x44_00758ae0() { std::free(head); } }; }
int main() { std::list_T_0x44_00758ae0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00758ae0_TEST PASS\n" : "00758ae0_TEST FAIL\n"); return ok ? 0 : 1; }
