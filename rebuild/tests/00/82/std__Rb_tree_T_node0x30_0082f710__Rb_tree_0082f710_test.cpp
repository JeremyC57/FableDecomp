#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x30_0082f710 { void* head; unsigned long size; _Rb_tree_T_node0x30_0082f710() : head(0), size(0) { head = std::malloc(0x30); } ~_Rb_tree_T_node0x30_0082f710() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x30_0082f710 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "0082f710_TEST PASS\n" : "0082f710_TEST FAIL\n"); return ok ? 0 : 1; }
