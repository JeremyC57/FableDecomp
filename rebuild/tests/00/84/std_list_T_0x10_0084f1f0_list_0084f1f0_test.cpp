#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x10_0084f1f0 { void* head; unsigned long size; list_T_0x10_0084f1f0() : head(0), size(0) { head = std::malloc(0x18); } ~list_T_0x10_0084f1f0() { std::free(head); } }; }
int main() { std::list_T_0x10_0084f1f0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "0084f1f0_TEST PASS\n" : "0084f1f0_TEST FAIL\n"); return ok ? 0 : 1; }
