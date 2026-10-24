#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x1c_00a2ac00 { void* head; unsigned long size; _Rb_tree_base_node0x1c_00a2ac00() : head(0), size(0) { head = std::malloc(0x1c); } ~_Rb_tree_base_node0x1c_00a2ac00() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x1c_00a2ac00 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00a2ac00_TEST PASS\n" : "00a2ac00_TEST FAIL\n"); return ok ? 0 : 1; }
