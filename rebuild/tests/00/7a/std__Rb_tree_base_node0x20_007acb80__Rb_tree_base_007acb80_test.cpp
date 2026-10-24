#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x20_007acb80 { void* head; unsigned long size; _Rb_tree_base_node0x20_007acb80() : head(0), size(0) { head = std::malloc(0x20); } ~_Rb_tree_base_node0x20_007acb80() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x20_007acb80 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "007acb80_TEST PASS\n" : "007acb80_TEST FAIL\n"); return ok ? 0 : 1; }
