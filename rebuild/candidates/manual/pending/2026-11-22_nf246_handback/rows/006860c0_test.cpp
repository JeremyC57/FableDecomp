// Behaviour test for 0x006860C0 std::list<const CThingAICreature*>::list(const allocator&) on the shared header: the constructed list
// owns one sentinel node linked to itself (empty), and takes elements afterwards.
#include "stlport/fable_containers.h"
#include <stdio.h>
class CThingAICreature;
typedef std::list<const CThingAICreature*, std::allocator<const CThingAICreature*> > CreatureList;
int main()
{
    std::allocator<const CThingAICreature*> a;
    CreatureList l(a);
    std::_List_node_base* s = l._M_node._M_data;
    if (s == 0 || s->_M_next != s || s->_M_prev != s || !l.empty()) { printf("FAIL 1\n"); return 1; }
    l.push_back((const CThingAICreature*)0);
    if (l.empty() || s->_M_next == s) { printf("FAIL 2\n"); return 2; }
    puts("LIST_SENTINEL_CTOR_006860c0_PASS");
    return 0;
}
