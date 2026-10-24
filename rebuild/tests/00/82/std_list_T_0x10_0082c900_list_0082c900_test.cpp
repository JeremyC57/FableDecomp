#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x10_0082c900 { void* head; unsigned long size; list_T_0x10_0082c900() : head(0), size(0) { head = std::malloc(0x18); } ~list_T_0x10_0082c900() { std::free(head); } }; }
int main() { std::list_T_0x10_0082c900 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "0082c900_TEST PASS\n" : "0082c900_TEST FAIL\n"); return ok ? 0 : 1; }
