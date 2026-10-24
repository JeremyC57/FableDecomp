#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x14_009d7b40 { void* head; unsigned long size; _Rb_tree_T_node0x14_009d7b40() : head(0), size(0) { head = std::malloc(0x14); } ~_Rb_tree_T_node0x14_009d7b40() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x14_009d7b40 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "009d7b40_TEST PASS\n" : "009d7b40_TEST FAIL\n"); return ok ? 0 : 1; }
