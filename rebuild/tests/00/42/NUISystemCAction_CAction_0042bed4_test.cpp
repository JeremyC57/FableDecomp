// behaviour test for 0x0042BED4
#include <stdio.h>
#include <stdlib.h>
namespace std {
template <class T> class allocator {
public:
    allocator() {}
};
template <class T, class A = allocator<T> > class list_0042abaf {
public:
    explicit list_0042abaf(const A& a = A());
    void* _M_node;
};
}
static int g_listCtors = 0;
struct ListNode { ListNode* next; ListNode* prev; void* data; };
template <class T, class A> std::list_0042abaf<T, A>::list_0042abaf(const A&)
{
    ++g_listCtors;
    ListNode* n = (ListNode*)malloc(sizeof(ListNode)); n->next = n->prev = n; _M_node = n;
}
namespace NUISystem {
class CActionParam;
enum EActionType { ACTION_NONE };
class CAction {
public:
    EActionType                ActionType;               // +0x00
    std::list_0042abaf<CActionParam*>   UIActionParams;           // +0x04
    CAction();
};
CAction::CAction()
{
}
}
int main()
{
    NUISystem::CAction a; int ok = 1;
    ListNode* n = (ListNode*)a.UIActionParams._M_node;
    ok &= g_listCtors == 1 && n != 0 && n->next == n && n->prev == n && sizeof(a) == 8;
    printf(ok ? "0042bed4_TEST PASS\n" : "0042bed4_TEST FAIL\n");
    return ok ? 0 : 1;
}