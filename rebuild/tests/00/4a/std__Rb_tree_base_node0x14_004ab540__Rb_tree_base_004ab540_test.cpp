#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x14_004ab540 { void* head; unsigned long size; _Rb_tree_base_node0x14_004ab540() : head(0), size(0) { head = std::malloc(0x14); } ~_Rb_tree_base_node0x14_004ab540() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x14_004ab540 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "004ab540_TEST PASS\n" : "004ab540_TEST FAIL\n"); return ok ? 0 : 1; }
