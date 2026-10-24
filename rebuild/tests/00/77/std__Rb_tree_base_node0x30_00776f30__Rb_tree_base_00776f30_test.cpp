#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x30_00776f30 { void* head; unsigned long size; _Rb_tree_base_node0x30_00776f30() : head(0), size(0) { head = std::malloc(0x30); } ~_Rb_tree_base_node0x30_00776f30() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x30_00776f30 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00776f30_TEST PASS\n" : "00776f30_TEST FAIL\n"); return ok ? 0 : 1; }
