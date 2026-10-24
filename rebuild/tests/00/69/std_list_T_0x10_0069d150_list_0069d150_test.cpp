#include <cstdio>
#include <cstdlib>
namespace std { struct list_T_0x10_0069d150 { void* head; unsigned long size; list_T_0x10_0069d150() : head(0), size(0) { head = std::malloc(0x18); } ~list_T_0x10_0069d150() { std::free(head); } }; }
int main() { std::list_T_0x10_0069d150 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "0069d150_TEST PASS\n" : "0069d150_TEST FAIL\n"); return ok ? 0 : 1; }
