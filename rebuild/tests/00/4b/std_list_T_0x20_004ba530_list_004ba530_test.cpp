#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x20_004ba530 { void* head; unsigned long size; list_T_0x20_004ba530() : head(0), size(0) { head = std::malloc(0x28); } ~list_T_0x20_004ba530() { std::free(head); } }; }
int main() { std::list_T_0x20_004ba530 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "004ba530_TEST PASS\n" : "004ba530_TEST FAIL\n"); return ok ? 0 : 1; }
