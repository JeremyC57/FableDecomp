// Behaviour test for 0x004046B0 std::_List_base<CStringParserCommentBoundaries>::~_List_base on the shared headers: clear() unlinks and
// frees every node and leaves the sentinel linked to itself; the destructor then frees the sentinel (a populated and an empty list).
#define FABLE_RETAIL_CSTRINGPARSER_COMMENT_BOUNDARIES_ONLY
#include "engine/CStringParser.h"
#include "stlport/fable_containers.h"
#include <stdio.h>
typedef std::list<CStringParserCommentBoundaries, std::allocator<CStringParserCommentBoundaries> > CommentList;
int main()
{
    static const char text[] = "a /* b */ c // d";
    {
        CommentList l;
        for (int i = 0; i < 3; ++i) {
            CStringParserCommentBoundaries c;
            c.PStart = text + i;
            c.PEnd = text + i + 4;
            l.push_back(c);
        }
        if (l.empty()) { printf("004046b0_TEST FAIL 1\n"); return 1; }
        std::_List_node_base* s = l._M_node._M_data;
        l.clear();
        if (!l.empty() || s->_M_next != s || s->_M_prev != s) { printf("004046b0_TEST FAIL 2\n"); return 2; }
        l.push_back(CStringParserCommentBoundaries());
    }   // ~list -> ~_List_base: one node and the sentinel freed
    {
        CommentList e;
        if (!e.empty()) { printf("004046b0_TEST FAIL 3\n"); return 3; }
    }   // empty: only the sentinel freed
    printf("004046b0_TEST PASS\n");
    return 0;
}
