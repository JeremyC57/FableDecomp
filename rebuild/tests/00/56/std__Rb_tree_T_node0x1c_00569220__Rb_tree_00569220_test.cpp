#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x1c_00569220 { void* head; unsigned long size; _Rb_tree_T_node0x1c_00569220() : head(0), size(0) { head = std::malloc(0x1c); } ~_Rb_tree_T_node0x1c_00569220() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x1c_00569220 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00569220_TEST PASS\n" : "00569220_TEST FAIL\n"); return ok ? 0 : 1; }
