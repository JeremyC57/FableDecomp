#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_T_node0x14_009aac60 { void* head; unsigned long size; _Rb_tree_T_node0x14_009aac60() : head(0), size(0) { head = std::malloc(0x14); } ~_Rb_tree_T_node0x14_009aac60() { std::free(head); } }; }
int main() { std::_Rb_tree_T_node0x14_009aac60 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "009aac60_TEST PASS\n" : "009aac60_TEST FAIL\n"); return ok ? 0 : 1; }
