#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x14_00835680 { void* head; unsigned long size; _Rb_tree_T_node0x14_00835680() : head(0), size(0) { head = std::malloc(0x14); } ~_Rb_tree_T_node0x14_00835680() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x14_00835680 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00835680_TEST PASS\n" : "00835680_TEST FAIL\n"); return ok ? 0 : 1; }
