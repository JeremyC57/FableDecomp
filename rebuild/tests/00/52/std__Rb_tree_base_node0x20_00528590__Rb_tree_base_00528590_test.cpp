#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x20_00528590 { void* head; unsigned long size; _Rb_tree_base_node0x20_00528590() : head(0), size(0) { head = std::malloc(0x20); } ~_Rb_tree_base_node0x20_00528590() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x20_00528590 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00528590_TEST PASS\n" : "00528590_TEST FAIL\n"); return ok ? 0 : 1; }
