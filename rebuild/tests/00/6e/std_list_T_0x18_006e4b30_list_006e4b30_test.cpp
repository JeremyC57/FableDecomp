#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x18_006e4b30 { void* head; unsigned long size; list_T_0x18_006e4b30() : head(0), size(0) { head = std::malloc(0x20); } ~list_T_0x18_006e4b30() { std::free(head); } }; }
int main() { std::list_T_0x18_006e4b30 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "006e4b30_TEST PASS\n" : "006e4b30_TEST FAIL\n"); return ok ? 0 : 1; }
