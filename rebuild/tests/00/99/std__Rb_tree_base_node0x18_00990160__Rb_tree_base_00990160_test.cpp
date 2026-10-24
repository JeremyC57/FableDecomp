#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x18_00990160 { void* head; unsigned long size; _Rb_tree_base_node0x18_00990160() : head(0), size(0) { head = std::malloc(0x18); } ~_Rb_tree_base_node0x18_00990160() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x18_00990160 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00990160_TEST PASS\n" : "00990160_TEST FAIL\n"); return ok ? 0 : 1; }
