// Behaviour test for 0x004046F0 std::list<CStringParserCommentBoundaries>::list(const allocator&) on the shared headers:
// the new list's sentinel is a fresh node linked to itself (an empty list).
#define FABLE_RETAIL_CSTRINGPARSER_COMMENT_BOUNDARIES_ONLY
#include "engine/CStringParser.h"
#include "stlport/fable_containers.h"
#include <stdio.h>
int main()
{
    std::allocator<CStringParserCommentBoundaries> a;
    std::list<CStringParserCommentBoundaries, std::allocator<CStringParserCommentBoundaries> > b(a);
    std::_List_node_base* n = b._M_node._M_data;
    if (!n) { printf("LIST_NODE_CREATE_CIRCULAR_TEST FAIL 1\n"); return 1; }
    if (n->_M_next != n || n->_M_prev != n) { printf("LIST_NODE_CREATE_CIRCULAR_TEST FAIL 2\n"); return 2; }
    printf("LIST_NODE_CREATE_CIRCULAR_TEST PASS\n");
    return 0;
}
