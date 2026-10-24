#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x1c_00553490 { void* head; unsigned long size; _Rb_tree_base_node0x1c_00553490() : head(0), size(0) { head = std::malloc(0x1c); } ~_Rb_tree_base_node0x1c_00553490() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x1c_00553490 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00553490_TEST PASS\n" : "00553490_TEST FAIL\n"); return ok ? 0 : 1; }
