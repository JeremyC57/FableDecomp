#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x38_009f8740 { void* head; unsigned long size; list_T_0x38_009f8740() : head(0), size(0) { head = std::malloc(0x40); } ~list_T_0x38_009f8740() { std::free(head); } }; }
int main() { std::list_T_0x38_009f8740 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "009f8740_TEST PASS\n" : "009f8740_TEST FAIL\n"); return ok ? 0 : 1; }
