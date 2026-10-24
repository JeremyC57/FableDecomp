#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x28_00a22360 { void* head; unsigned long size; _Rb_tree_T_node0x28_00a22360() : head(0), size(0) { head = std::malloc(0x28); } ~_Rb_tree_T_node0x28_00a22360() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x28_00a22360 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00a22360_TEST PASS\n" : "00a22360_TEST FAIL\n"); return ok ? 0 : 1; }
