#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x14_009ac380 { void* head; unsigned long size; _Rb_tree_T_node0x14_009ac380() : head(0), size(0) { head = std::malloc(0x14); } ~_Rb_tree_T_node0x14_009ac380() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x14_009ac380 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "009ac380_TEST PASS\n" : "009ac380_TEST FAIL\n"); return ok ? 0 : 1; }
