#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x18_005e8e50 { void* head; unsigned long size; _Rb_tree_base_node0x18_005e8e50() : head(0), size(0) { head = std::malloc(0x18); } ~_Rb_tree_base_node0x18_005e8e50() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x18_005e8e50 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "005e8e50_TEST PASS\n" : "005e8e50_TEST FAIL\n"); return ok ? 0 : 1; }
