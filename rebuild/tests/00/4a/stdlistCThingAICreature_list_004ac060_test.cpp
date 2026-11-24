// Behaviour test for 0x004AC060 std::list<CThingAICreature*>::list(const allocator&) on the shared header: the constructed list
// owns one sentinel node linked to itself (empty), and takes elements afterwards.
#include "stlport/fable_containers.h"
#include <stdio.h>
class CThingAICreature;
typedef std::list<CThingAICreature*, std::allocator<CThingAICreature*> > CreatureList;
int main()
{
    std::allocator<CThingAICreature*> a;
    CreatureList l(a);
    std::_List_node_base* s = l._M_node._M_data;
    if (s == 0 || s->_M_next != s || s->_M_prev != s || !l.empty()) { printf("FAIL 1\n"); return 1; }
    l.push_back((CThingAICreature*)0);
    if (l.empty() || s->_M_next == s) { printf("FAIL 2\n"); return 2; }
    puts("LIST_SENTINEL_CTOR_004ac060_PASS");
    return 0;
}
