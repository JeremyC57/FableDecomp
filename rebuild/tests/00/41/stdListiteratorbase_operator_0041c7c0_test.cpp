// Behaviour test for 0x0041C7C0 std::_List_iterator_base::operator!= (NF146, 2026-11-20): the shared stlport/fable_containers.h body the
//   row emits; equal and different nodes. Keeps the catalog PassPattern.
#include <stdio.h>
#include "stlport/fable_containers.h"
int main(void)
{
    std::_List_node_base a, b;
    std::_List_iterator_base i(&a), j(&a), k(&b);
    int ok = !(i != j) && (i != k) && (k != j);
    printf(ok ? "PASS_0041c7c0\n" : "FAIL_0041c7c0\n");
    return ok ? 0 : 1;
}
