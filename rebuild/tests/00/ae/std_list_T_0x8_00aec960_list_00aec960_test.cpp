#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x8_00aec960 { void* head; unsigned long size; list_T_0x8_00aec960() : head(0), size(0) { head = std::malloc(0x10); } ~list_T_0x8_00aec960() { std::free(head); } }; }
int main() { std::list_T_0x8_00aec960 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00aec960_TEST PASS\n" : "00aec960_TEST FAIL\n"); return ok ? 0 : 1; }
