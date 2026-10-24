#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x18_006c7d30 { void* head; unsigned long size; _Rb_tree_T_node0x18_006c7d30() : head(0), size(0) { head = std::malloc(0x18); } ~_Rb_tree_T_node0x18_006c7d30() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x18_006c7d30 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "006c7d30_TEST PASS\n" : "006c7d30_TEST FAIL\n"); return ok ? 0 : 1; }
