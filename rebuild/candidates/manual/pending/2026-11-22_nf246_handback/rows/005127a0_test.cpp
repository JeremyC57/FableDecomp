// Behaviour test for 0x005127A0 std::_List_base<C2DLineF>::~_List_base on the shared headers: clear() unlinks and frees every node and
// leaves the sentinel linked to itself; the destructor then frees the sentinel (a populated and an empty list).
#include "engine/C2DLineF.h"
#include "stlport/fable_containers.h"
#include <stdio.h>
typedef std::list<C2DLineF, std::allocator<C2DLineF> > LineList;
int main()
{
    {
        LineList l;
        for (int i = 0; i < 3; ++i) {
            C2DLineF line;
            line.Point1.X = (float)i; line.Point1.Y = 1.0f;
            line.Point2.X = 2.0f; line.Point2.Y = (float)(i + 3);
            l.push_back(line);
        }
        if (l.empty()) { printf("005127a0_TEST FAIL 1\n"); return 1; }
        std::_List_node_base* s = l._M_node._M_data;
        l.clear();
        if (!l.empty() || s->_M_next != s || s->_M_prev != s) { printf("005127a0_TEST FAIL 2\n"); return 2; }
        l.push_back(C2DLineF());
    }   // ~list -> ~_List_base: one node and the sentinel freed
    {
        LineList e;
        if (!e.empty() || sizeof(LineList) != 4) { printf("005127a0_TEST FAIL 3\n"); return 3; }
    }   // empty: only the sentinel freed
    printf("005127a0_TEST PASS\n");
    return 0;
}
