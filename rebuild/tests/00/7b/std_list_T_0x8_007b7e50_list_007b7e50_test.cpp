#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x8_007b7e50 { void* head; unsigned long size; list_T_0x8_007b7e50() : head(0), size(0) { head = std::malloc(0x10); } ~list_T_0x8_007b7e50() { std::free(head); } }; }
int main() { std::list_T_0x8_007b7e50 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "007b7e50_TEST PASS\n" : "007b7e50_TEST FAIL\n"); return ok ? 0 : 1; }
