#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x20_00528480 { void* head; unsigned long size; _Rb_tree_base_node0x20_00528480() : head(0), size(0) { head = std::malloc(0x20); } ~_Rb_tree_base_node0x20_00528480() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x20_00528480 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00528480_TEST PASS\n" : "00528480_TEST FAIL\n"); return ok ? 0 : 1; }
