// behaviour test for 0x0042BED4
#include <stdio.h>
#include <stdlib.h>
namespace std {
template <class T> class allocator {
public:
    allocator() {}
};
template <class T, class A = allocator<T> > class list {
public:
    explicit list(const A& a = A());
    void* _M_node;
};
}
static int g_listCtors = 0;
struct ListNode { ListNode* next; ListNode* prev; void* data; };
template <class T, class A> std::list<T, A>::list(const A&)
{
    ++g_listCtors;
    ListNode* n = (ListNode*)malloc(sizeof(ListNode)); n->next = n->prev = n; _M_node = n;
}
namespace NUISystem {
class CComponent;
enum EDeletionMethod { DELETION_METHOD_DEFAULT };
class CDeletion {
public:
    EDeletionMethod            Method;               // +0x00
    std::list<CComponent*>     AssociatedParents;           // +0x04
    CDeletion();
};
CDeletion::CDeletion()
{
}
}
int main()
{
    NUISystem::CDeletion a; int ok = 1;
    ListNode* n = (ListNode*)a.AssociatedParents._M_node;
    ok &= g_listCtors == 1 && n != 0 && n->next == n && n->prev == n && sizeof(a) == 8;
    printf(ok ? "0042bed4_TEST PASS\n" : "0042bed4_TEST FAIL\n");
    return ok ? 0 : 1;
}