#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x28_0070a960 { void* head; unsigned long size; _Rb_tree_T_node0x28_0070a960() : head(0), size(0) { head = std::malloc(0x28); } ~_Rb_tree_T_node0x28_0070a960() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x28_0070a960 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "0070a960_TEST PASS\n" : "0070a960_TEST FAIL\n"); return ok ? 0 : 1; }
