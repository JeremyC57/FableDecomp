#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x20_00a16cf0 { void* head; unsigned long size; list_T_0x20_00a16cf0() : head(0), size(0) { head = std::malloc(0x28); } ~list_T_0x20_00a16cf0() { std::free(head); } }; }
int main() { std::list_T_0x20_00a16cf0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00a16cf0_TEST PASS\n" : "00a16cf0_TEST FAIL\n"); return ok ? 0 : 1; }
