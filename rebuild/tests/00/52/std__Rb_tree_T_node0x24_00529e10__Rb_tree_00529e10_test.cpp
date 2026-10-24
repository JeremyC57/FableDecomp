#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x24_00529e10 { void* head; unsigned long size; _Rb_tree_T_node0x24_00529e10() : head(0), size(0) { head = std::malloc(0x24); } ~_Rb_tree_T_node0x24_00529e10() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x24_00529e10 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00529e10_TEST PASS\n" : "00529e10_TEST FAIL\n"); return ok ? 0 : 1; }
