#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x8_00535930 { void* head; unsigned long size; list_T_0x8_00535930() : head(0), size(0) { head = std::malloc(0x10); } ~list_T_0x8_00535930() { std::free(head); } }; }
int main() { std::list_T_0x8_00535930 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00535930_TEST PASS\n" : "00535930_TEST FAIL\n"); return ok ? 0 : 1; }
