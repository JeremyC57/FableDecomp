#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x20_00a17530 { void* head; unsigned long size; list_T_0x20_00a17530() : head(0), size(0) { head = std::malloc(0x28); } ~list_T_0x20_00a17530() { std::free(head); } }; }
int main() { std::list_T_0x20_00a17530 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00a17530_TEST PASS\n" : "00a17530_TEST FAIL\n"); return ok ? 0 : 1; }
