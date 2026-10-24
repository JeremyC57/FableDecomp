#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x60_009f9c00 { void* head; unsigned long size; list_T_0x60_009f9c00() : head(0), size(0) { head = std::malloc(0x68); } ~list_T_0x60_009f9c00() { std::free(head); } }; }
int main() { std::list_T_0x60_009f9c00 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "009f9c00_TEST PASS\n" : "009f9c00_TEST FAIL\n"); return ok ? 0 : 1; }
