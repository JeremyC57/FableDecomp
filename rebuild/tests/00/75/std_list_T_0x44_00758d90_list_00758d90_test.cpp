#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x44_00758d90 { void* head; unsigned long size; list_T_0x44_00758d90() : head(0), size(0) { head = std::malloc(0x4c); } ~list_T_0x44_00758d90() { std::free(head); } }; }
int main() { std::list_T_0x44_00758d90 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00758d90_TEST PASS\n" : "00758d90_TEST FAIL\n"); return ok ? 0 : 1; }
