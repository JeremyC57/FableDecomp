#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x2c_007977e0 { void* head; unsigned long size; _Rb_tree_base_node0x2c_007977e0() : head(0), size(0) { head = std::malloc(0x2c); } ~_Rb_tree_base_node0x2c_007977e0() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x2c_007977e0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "007977e0_TEST PASS\n" : "007977e0_TEST FAIL\n"); return ok ? 0 : 1; }
