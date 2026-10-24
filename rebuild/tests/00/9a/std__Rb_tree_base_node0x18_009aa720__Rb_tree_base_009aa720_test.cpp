#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x18_009aa720 { void* head; unsigned long size; _Rb_tree_base_node0x18_009aa720() : head(0), size(0) { head = std::malloc(0x18); } ~_Rb_tree_base_node0x18_009aa720() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x18_009aa720 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "009aa720_TEST PASS\n" : "009aa720_TEST FAIL\n"); return ok ? 0 : 1; }
