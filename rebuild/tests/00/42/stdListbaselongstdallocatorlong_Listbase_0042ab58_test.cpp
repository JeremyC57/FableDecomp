// Behaviour test for std::_List_base<long, std::allocator<long> >::_List_base(const allocator<long>&) (0x0042AB58): the
// proxy pointer starts null, then one node is allocated and linked to itself (an empty circular list).
#include <cstdio>
#include <cstdlib>
struct ListNode { ListNode* next; ListNode* previous; long data; };
struct Allocator { Allocator() { } };
struct ListBase {
    ListNode* node;
    explicit ListBase(const Allocator&) : node(0)
    {
        ListNode* n = (ListNode*)std::malloc(sizeof(ListNode));
        n->next = n;
        n->previous = n;
        node = n;
    }
};
int main()
{
    ListBase list((Allocator()));
    bool ok = list.node != 0 && list.node->next == list.node && list.node->previous == list.node && sizeof(ListBase) == sizeof(void*);
    std::free(list.node);
    std::printf(ok ? "0042ab58_TEST PASS\n" : "0042ab58_TEST FAIL\n");
    return ok ? 0 : 1;
}
