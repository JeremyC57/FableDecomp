#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x20_0048cac0 { void* head; unsigned long size; _Rb_tree_base_node0x20_0048cac0() : head(0), size(0) { head = std::malloc(0x20); } ~_Rb_tree_base_node0x20_0048cac0() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x20_0048cac0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "0048cac0_TEST PASS\n" : "0048cac0_TEST FAIL\n"); return ok ? 0 : 1; }
