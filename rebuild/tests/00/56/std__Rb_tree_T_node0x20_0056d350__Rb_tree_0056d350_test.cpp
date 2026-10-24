#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x20_0056d350 { void* head; unsigned long size; _Rb_tree_T_node0x20_0056d350() : head(0), size(0) { head = std::malloc(0x20); } ~_Rb_tree_T_node0x20_0056d350() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x20_0056d350 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "0056d350_TEST PASS\n" : "0056d350_TEST FAIL\n"); return ok ? 0 : 1; }
