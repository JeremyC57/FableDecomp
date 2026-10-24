#include <cstdio>
#include <cstdlib>
namespace std { struct _Rb_tree_base_node0x24_00528520 { void* head; unsigned long size; _Rb_tree_base_node0x24_00528520() : head(0), size(0) { head = std::malloc(0x24); } ~_Rb_tree_base_node0x24_00528520() { std::free(head); } }; }
int main() { std::_Rb_tree_base_node0x24_00528520 c; bool ok = c.head != 0 && c.size == 0; std::printf(ok ? "00528520_TEST PASS\n" : "00528520_TEST FAIL\n"); return ok ? 0 : 1; }
