#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x8_00743200 { void* head; unsigned long size; list_T_0x8_00743200() : head(0), size(0) { head = std::malloc(0x10); } ~list_T_0x8_00743200() { std::free(head); } }; }
int main() { std::list_T_0x8_00743200 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00743200_TEST PASS\n" : "00743200_TEST FAIL\n"); return ok ? 0 : 1; }
