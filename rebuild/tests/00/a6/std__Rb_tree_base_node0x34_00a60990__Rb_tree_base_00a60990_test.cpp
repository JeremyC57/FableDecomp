#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x34_00a60990 { void* head; unsigned long size; _Rb_tree_base_node0x34_00a60990() : head(0), size(0) { head = std::malloc(0x34); } ~_Rb_tree_base_node0x34_00a60990() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x34_00a60990 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00a60990_TEST PASS\n" : "00a60990_TEST FAIL\n"); return ok ? 0 : 1; }
