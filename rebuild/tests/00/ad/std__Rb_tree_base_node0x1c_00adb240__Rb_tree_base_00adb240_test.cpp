#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x1c_00adb240 { void* head; unsigned long size; _Rb_tree_base_node0x1c_00adb240() : head(0), size(0) { head = std::malloc(0x1c); } ~_Rb_tree_base_node0x1c_00adb240() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x1c_00adb240 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00adb240_TEST PASS\n" : "00adb240_TEST FAIL\n"); return ok ? 0 : 1; }
