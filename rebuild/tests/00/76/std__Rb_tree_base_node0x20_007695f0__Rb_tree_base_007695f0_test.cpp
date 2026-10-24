#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x20_007695f0 { void* head; unsigned long size; _Rb_tree_base_node0x20_007695f0() : head(0), size(0) { head = std::malloc(0x20); } ~_Rb_tree_base_node0x20_007695f0() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x20_007695f0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "007695f0_TEST PASS\n" : "007695f0_TEST FAIL\n"); return ok ? 0 : 1; }
