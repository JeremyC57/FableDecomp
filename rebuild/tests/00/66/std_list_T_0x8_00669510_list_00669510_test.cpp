#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x8_00669510 { void* head; unsigned long size; list_T_0x8_00669510() : head(0), size(0) { head = std::malloc(0x10); } ~list_T_0x8_00669510() { std::free(head); } }; }
int main() { std::list_T_0x8_00669510 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00669510_TEST PASS\n" : "00669510_TEST FAIL\n"); return ok ? 0 : 1; }
