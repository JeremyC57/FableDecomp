#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x58_00687480 { void* head; unsigned long size; list_T_0x58_00687480() : head(0), size(0) { head = std::malloc(0x60); } ~list_T_0x58_00687480() { std::free(head); } }; }
int main() { std::list_T_0x58_00687480 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00687480_TEST PASS\n" : "00687480_TEST FAIL\n"); return ok ? 0 : 1; }
