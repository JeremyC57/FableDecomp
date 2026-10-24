#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x20_00528d80 { void* head; unsigned long size; _Rb_tree_T_node0x20_00528d80() : head(0), size(0) { head = std::malloc(0x20); } ~_Rb_tree_T_node0x20_00528d80() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x20_00528d80 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00528d80_TEST PASS\n" : "00528d80_TEST FAIL\n"); return ok ? 0 : 1; }
