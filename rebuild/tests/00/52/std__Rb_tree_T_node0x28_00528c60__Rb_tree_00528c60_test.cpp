#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x28_00528c60 { void* head; unsigned long size; _Rb_tree_T_node0x28_00528c60() : head(0), size(0) { head = std::malloc(0x28); } ~_Rb_tree_T_node0x28_00528c60() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x28_00528c60 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00528c60_TEST PASS\n" : "00528c60_TEST FAIL\n"); return ok ? 0 : 1; }
