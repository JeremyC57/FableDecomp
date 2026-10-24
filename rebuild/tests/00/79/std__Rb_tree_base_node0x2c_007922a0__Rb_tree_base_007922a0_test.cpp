#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x2c_007922a0 { void* head; unsigned long size; _Rb_tree_base_node0x2c_007922a0() : head(0), size(0) { head = std::malloc(0x2c); } ~_Rb_tree_base_node0x2c_007922a0() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x2c_007922a0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "007922a0_TEST PASS\n" : "007922a0_TEST FAIL\n"); return ok ? 0 : 1; }
