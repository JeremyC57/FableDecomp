// Behaviour test for 0x0041C7A0 std::_List_iterator_base::_M_incr (NF146, 2026-11-20): the shared stlport/fable_containers.h body the row
//   emits; steps through a three-node ring and back to the head. Keeps the catalog PassPattern.
#include <stdio.h>
#include "stlport/fable_containers.h"
int main(void)
{
    std::_List_node_base a, b, c;
    a._M_next = &b; b._M_next = &c; c._M_next = &a; a._M_prev = &c; b._M_prev = &a; c._M_prev = &b;
    std::_List_iterator_base it(&a);
    it._M_incr(); int ok = it._M_node == &b;
    it._M_incr(); ok = ok && it._M_node == &c;
    it._M_incr(); ok = ok && it._M_node == &a && a._M_next == &b;
    printf(ok ? "PASS_0041c7a0\n" : "FAIL_0041c7a0\n");
    return ok ? 0 : 1;
}
