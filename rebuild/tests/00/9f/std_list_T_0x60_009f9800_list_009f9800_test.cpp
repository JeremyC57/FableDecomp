#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x60_009f9800 { void* head; unsigned long size; list_T_0x60_009f9800() : head(0), size(0) { head = std::malloc(0x68); } ~list_T_0x60_009f9800() { std::free(head); } }; }
int main() { std::list_T_0x60_009f9800 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "009f9800_TEST PASS\n" : "009f9800_TEST FAIL\n"); return ok ? 0 : 1; }
