#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x24_009f0ce0 { void* head; unsigned long size; _Rb_tree_base_node0x24_009f0ce0() : head(0), size(0) { head = std::malloc(0x24); } ~_Rb_tree_base_node0x24_009f0ce0() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x24_009f0ce0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "009f0ce0_TEST PASS\n" : "009f0ce0_TEST FAIL\n"); return ok ? 0 : 1; }
