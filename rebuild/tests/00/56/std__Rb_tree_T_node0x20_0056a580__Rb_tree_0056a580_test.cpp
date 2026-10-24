#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x20_0056a580 { void* head; unsigned long size; _Rb_tree_T_node0x20_0056a580() : head(0), size(0) { head = std::malloc(0x20); } ~_Rb_tree_T_node0x20_0056a580() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x20_0056a580 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "0056a580_TEST PASS\n" : "0056a580_TEST FAIL\n"); return ok ? 0 : 1; }
