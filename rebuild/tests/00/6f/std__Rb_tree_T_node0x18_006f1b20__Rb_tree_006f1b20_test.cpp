#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x18_006f1b20 { void* head; unsigned long size; _Rb_tree_T_node0x18_006f1b20() : head(0), size(0) { head = std::malloc(0x18); } ~_Rb_tree_T_node0x18_006f1b20() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x18_006f1b20 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "006f1b20_TEST PASS\n" : "006f1b20_TEST FAIL\n"); return ok ? 0 : 1; }
