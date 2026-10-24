#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x18_0056d010 { void* head; unsigned long size; _Rb_tree_base_node0x18_0056d010() : head(0), size(0) { head = std::malloc(0x18); } ~_Rb_tree_base_node0x18_0056d010() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x18_0056d010 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "0056d010_TEST PASS\n" : "0056d010_TEST FAIL\n"); return ok ? 0 : 1; }
