#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x24_00b43bf0 { void* head; unsigned long size; _Rb_tree_base_node0x24_00b43bf0() : head(0), size(0) { head = std::malloc(0x24); } ~_Rb_tree_base_node0x24_00b43bf0() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x24_00b43bf0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00b43bf0_TEST PASS\n" : "00b43bf0_TEST FAIL\n"); return ok ? 0 : 1; }
