#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x1c_0069db20 { void* head; unsigned long size; _Rb_tree_T_node0x1c_0069db20() : head(0), size(0) { head = std::malloc(0x1c); } ~_Rb_tree_T_node0x1c_0069db20() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x1c_0069db20 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "0069db20_TEST PASS\n" : "0069db20_TEST FAIL\n"); return ok ? 0 : 1; }
