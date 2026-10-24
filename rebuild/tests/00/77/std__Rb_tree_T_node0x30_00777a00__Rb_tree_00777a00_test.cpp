#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x30_00777a00 { void* head; unsigned long size; _Rb_tree_T_node0x30_00777a00() : head(0), size(0) { head = std::malloc(0x30); } ~_Rb_tree_T_node0x30_00777a00() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x30_00777a00 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00777a00_TEST PASS\n" : "00777a00_TEST FAIL\n"); return ok ? 0 : 1; }
