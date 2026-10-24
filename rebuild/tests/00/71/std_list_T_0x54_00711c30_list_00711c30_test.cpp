#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x54_00711c30 { void* head; unsigned long size; list_T_0x54_00711c30() : head(0), size(0) { head = std::malloc(0x5c); } ~list_T_0x54_00711c30() { std::free(head); } }; }
int main() { std::list_T_0x54_00711c30 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00711c30_TEST PASS\n" : "00711c30_TEST FAIL\n"); return ok ? 0 : 1; }
