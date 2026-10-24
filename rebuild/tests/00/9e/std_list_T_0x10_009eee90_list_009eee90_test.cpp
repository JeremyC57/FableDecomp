#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x10_009eee90 { void* head; unsigned long size; list_T_0x10_009eee90() : head(0), size(0) { head = std::malloc(0x18); } ~list_T_0x10_009eee90() { std::free(head); } }; }
int main() { std::list_T_0x10_009eee90 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "009eee90_TEST PASS\n" : "009eee90_TEST FAIL\n"); return ok ? 0 : 1; }
