#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x28_009d8410 { void* head; unsigned long size; _Rb_tree_T_node0x28_009d8410() : head(0), size(0) { head = std::malloc(0x28); } ~_Rb_tree_T_node0x28_009d8410() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x28_009d8410 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "009d8410_TEST PASS\n" : "009d8410_TEST FAIL\n"); return ok ? 0 : 1; }
