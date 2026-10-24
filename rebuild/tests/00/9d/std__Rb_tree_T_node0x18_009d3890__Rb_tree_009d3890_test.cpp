#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x18_009d3890 { void* head; unsigned long size; _Rb_tree_T_node0x18_009d3890() : head(0), size(0) { head = std::malloc(0x18); } ~_Rb_tree_T_node0x18_009d3890() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x18_009d3890 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "009d3890_TEST PASS\n" : "009d3890_TEST FAIL\n"); return ok ? 0 : 1; }
