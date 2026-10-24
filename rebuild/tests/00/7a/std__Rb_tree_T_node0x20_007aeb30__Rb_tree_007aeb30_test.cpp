#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x20_007aeb30 { void* head; unsigned long size; _Rb_tree_T_node0x20_007aeb30() : head(0), size(0) { head = std::malloc(0x20); } ~_Rb_tree_T_node0x20_007aeb30() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x20_007aeb30 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "007aeb30_TEST PASS\n" : "007aeb30_TEST FAIL\n"); return ok ? 0 : 1; }
