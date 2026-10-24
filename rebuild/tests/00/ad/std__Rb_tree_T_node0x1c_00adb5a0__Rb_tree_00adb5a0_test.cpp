#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x1c_00adb5a0 { void* head; unsigned long size; _Rb_tree_T_node0x1c_00adb5a0() : head(0), size(0) { head = std::malloc(0x1c); } ~_Rb_tree_T_node0x1c_00adb5a0() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x1c_00adb5a0 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00adb5a0_TEST PASS\n" : "00adb5a0_TEST FAIL\n"); return ok ? 0 : 1; }
