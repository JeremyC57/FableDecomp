#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x34_00a611d0 { void* head; unsigned long size; _Rb_tree_T_node0x34_00a611d0() : head(0), size(0) { head = std::malloc(0x34); } ~_Rb_tree_T_node0x34_00a611d0() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x34_00a611d0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00a611d0_TEST PASS\n" : "00a611d0_TEST FAIL\n"); return ok ? 0 : 1; }
