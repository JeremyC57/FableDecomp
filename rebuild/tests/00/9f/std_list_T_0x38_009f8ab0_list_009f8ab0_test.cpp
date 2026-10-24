#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x38_009f8ab0 { void* head; unsigned long size; list_T_0x38_009f8ab0() : head(0), size(0) { head = std::malloc(0x40); } ~list_T_0x38_009f8ab0() { std::free(head); } }; }
int main() { std::list_T_0x38_009f8ab0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "009f8ab0_TEST PASS\n" : "009f8ab0_TEST FAIL\n"); return ok ? 0 : 1; }
