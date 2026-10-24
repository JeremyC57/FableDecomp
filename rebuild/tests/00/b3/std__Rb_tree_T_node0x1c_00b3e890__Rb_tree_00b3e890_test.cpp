#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x1c_00b3e890 { void* head; unsigned long size; _Rb_tree_T_node0x1c_00b3e890() : head(0), size(0) { head = std::malloc(0x1c); } ~_Rb_tree_T_node0x1c_00b3e890() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x1c_00b3e890 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00b3e890_TEST PASS\n" : "00b3e890_TEST FAIL\n"); return ok ? 0 : 1; }
