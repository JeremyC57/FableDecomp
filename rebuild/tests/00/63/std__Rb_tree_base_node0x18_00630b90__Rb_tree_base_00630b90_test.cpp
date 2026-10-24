#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x18_00630b90 { void* head; unsigned long size; _Rb_tree_base_node0x18_00630b90() : head(0), size(0) { head = std::malloc(0x18); } ~_Rb_tree_base_node0x18_00630b90() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x18_00630b90 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00630b90_TEST PASS\n" : "00630b90_TEST FAIL\n"); return ok ? 0 : 1; }
