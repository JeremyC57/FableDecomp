#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x1c_006bd640 { void* head; unsigned long size; _Rb_tree_T_node0x1c_006bd640() : head(0), size(0) { head = std::malloc(0x1c); } ~_Rb_tree_T_node0x1c_006bd640() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x1c_006bd640 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "006bd640_TEST PASS\n" : "006bd640_TEST FAIL\n"); return ok ? 0 : 1; }
