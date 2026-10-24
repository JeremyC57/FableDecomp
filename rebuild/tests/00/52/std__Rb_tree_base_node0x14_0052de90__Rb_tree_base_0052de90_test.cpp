#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x14_0052de90 { void* head; unsigned long size; _Rb_tree_base_node0x14_0052de90() : head(0), size(0) { head = std::malloc(0x14); } ~_Rb_tree_base_node0x14_0052de90() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x14_0052de90 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "0052de90_TEST PASS\n" : "0052de90_TEST FAIL\n"); return ok ? 0 : 1; }
