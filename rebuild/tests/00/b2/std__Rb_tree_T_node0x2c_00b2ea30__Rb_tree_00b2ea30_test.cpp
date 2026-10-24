#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x2c_00b2ea30 { void* head; unsigned long size; _Rb_tree_T_node0x2c_00b2ea30() : head(0), size(0) { head = std::malloc(0x2c); } ~_Rb_tree_T_node0x2c_00b2ea30() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x2c_00b2ea30 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00b2ea30_TEST PASS\n" : "00b2ea30_TEST FAIL\n"); return ok ? 0 : 1; }
