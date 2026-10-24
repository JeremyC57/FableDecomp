#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x28_009ac0f0 { void* head; unsigned long size; _Rb_tree_T_node0x28_009ac0f0() : head(0), size(0) { head = std::malloc(0x28); } ~_Rb_tree_T_node0x28_009ac0f0() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x28_009ac0f0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "009ac0f0_TEST PASS\n" : "009ac0f0_TEST FAIL\n"); return ok ? 0 : 1; }
