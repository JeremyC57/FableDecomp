#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x20_00ad2360 { void* head; unsigned long size; _Rb_tree_base_node0x20_00ad2360() : head(0), size(0) { head = std::malloc(0x20); } ~_Rb_tree_base_node0x20_00ad2360() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x20_00ad2360 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00ad2360_TEST PASS\n" : "00ad2360_TEST FAIL\n"); return ok ? 0 : 1; }
