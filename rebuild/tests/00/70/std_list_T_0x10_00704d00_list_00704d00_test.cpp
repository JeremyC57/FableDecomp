#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x10_00704d00 { void* head; unsigned long size; list_T_0x10_00704d00() : head(0), size(0) { head = std::malloc(0x18); } ~list_T_0x10_00704d00() { std::free(head); } }; }
int main() { std::list_T_0x10_00704d00 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00704d00_TEST PASS\n" : "00704d00_TEST FAIL\n"); return ok ? 0 : 1; }
