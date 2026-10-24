#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x18_007e9cc0 { void* head; unsigned long size; _Rb_tree_T_node0x18_007e9cc0() : head(0), size(0) { head = std::malloc(0x18); } ~_Rb_tree_T_node0x18_007e9cc0() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x18_007e9cc0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "007e9cc0_TEST PASS\n" : "007e9cc0_TEST FAIL\n"); return ok ? 0 : 1; }
