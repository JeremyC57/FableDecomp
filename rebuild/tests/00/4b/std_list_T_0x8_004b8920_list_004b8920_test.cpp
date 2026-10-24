#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x8_004b8920 { void* head; unsigned long size; list_T_0x8_004b8920() : head(0), size(0) { head = std::malloc(0x10); } ~list_T_0x8_004b8920() { std::free(head); } }; }
int main() { std::list_T_0x8_004b8920 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "004b8920_TEST PASS\n" : "004b8920_TEST FAIL\n"); return ok ? 0 : 1; }
