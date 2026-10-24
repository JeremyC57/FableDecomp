#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x18_006db770 { void* head; unsigned long size; _Rb_tree_T_node0x18_006db770() : head(0), size(0) { head = std::malloc(0x18); } ~_Rb_tree_T_node0x18_006db770() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x18_006db770 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "006db770_TEST PASS\n" : "006db770_TEST FAIL\n"); return ok ? 0 : 1; }
