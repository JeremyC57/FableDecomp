#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x24_00cdbf70 { void* head; unsigned long size; _Rb_tree_T_node0x24_00cdbf70() : head(0), size(0) { head = std::malloc(0x24); } ~_Rb_tree_T_node0x24_00cdbf70() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x24_00cdbf70 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00cdbf70_TEST PASS\n" : "00cdbf70_TEST FAIL\n"); return ok ? 0 : 1; }
