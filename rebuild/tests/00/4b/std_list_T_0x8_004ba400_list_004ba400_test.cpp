#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x8_004ba400 { void* head; unsigned long size; list_T_0x8_004ba400() : head(0), size(0) { head = std::malloc(0x10); } ~list_T_0x8_004ba400() { std::free(head); } }; }
int main() { std::list_T_0x8_004ba400 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "004ba400_TEST PASS\n" : "004ba400_TEST FAIL\n"); return ok ? 0 : 1; }
