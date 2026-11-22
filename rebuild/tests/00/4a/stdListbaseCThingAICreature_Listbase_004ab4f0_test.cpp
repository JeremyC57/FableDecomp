// Behaviour test for 0x004AB4F0 std::_List_base<CThingAICreature*>::~_List_base on the shared header: clear() unlinks and frees every
// node and leaves the sentinel linked to itself; the destructor then frees the sentinel (a populated and an empty list).
#include "stlport/fable_containers.h"
#include <stdio.h>
class CThingAICreature;
typedef std::list<CThingAICreature*, std::allocator<CThingAICreature*> > CreatureList;
int main()
{
    {
        CreatureList l;
        for (int i = 0; i < 3; ++i) l.push_back((CThingAICreature*)0);
        if (l.empty()) { printf("FAIL_004ab4f0 1\n"); return 1; }
        std::_List_node_base* s = l._M_node._M_data;
        l.clear();
        if (!l.empty() || s->_M_next != s || s->_M_prev != s) { printf("FAIL_004ab4f0 2\n"); return 2; }
        l.push_back((CThingAICreature*)0);
    }   // ~list -> ~_List_base: one node and the sentinel freed
    {
        CreatureList e;
        if (!e.empty() || sizeof(CreatureList) != 4) { printf("FAIL_004ab4f0 3\n"); return 3; }
    }
    printf("PASS_004ab4f0\n");
    return 0;
}
