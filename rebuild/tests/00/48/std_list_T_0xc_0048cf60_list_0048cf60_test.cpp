#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0xc_0048cf60 { void* head; unsigned long size; list_T_0xc_0048cf60() : head(0), size(0) { head = std::malloc(0x14); } ~list_T_0xc_0048cf60() { std::free(head); } }; }
int main() { std::list_T_0xc_0048cf60 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "0048cf60_TEST PASS\n" : "0048cf60_TEST FAIL\n"); return ok ? 0 : 1; }
