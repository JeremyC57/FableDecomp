#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x1c_0098ca70 { void* head; unsigned long size; _Rb_tree_base_node0x1c_0098ca70() : head(0), size(0) { head = std::malloc(0x1c); } ~_Rb_tree_base_node0x1c_0098ca70() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x1c_0098ca70 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "0098ca70_TEST PASS\n" : "0098ca70_TEST FAIL\n"); return ok ? 0 : 1; }
