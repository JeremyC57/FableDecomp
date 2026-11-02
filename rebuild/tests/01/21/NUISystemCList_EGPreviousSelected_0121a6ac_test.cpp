// Behaviour test for 0x0121A6AC: the static list NUISystem::CList::GPreviousSelected starts empty -- one self-linked sentinel node.
#include <cstdio>
#include <cstdlib>
struct ListNodeBase { ListNodeBase* Next; ListNodeBase* Prev; };
struct ListNode : ListNodeBase { void* Data; };
class CListModel {                                       // STLport 4.6 list<T*>: the sentinel pointer only
public:
    CListModel() { ListNode* n = (ListNode*)std::malloc(sizeof(ListNode)); n->Next = n; n->Prev = n; Node = n; }
    ~CListModel() { std::free(Node); }
    bool empty() const { return Node->Next == Node; }
    ListNode* Node;
};
namespace NUISystem {
class CList { public: static CListModel GPreviousSelected; };
}
CListModel NUISystem::CList::GPreviousSelected;
int main()
{
    ListNode* n = NUISystem::CList::GPreviousSelected.Node;
    if (sizeof(void*) == 4 && sizeof(ListNode) != 0xc) { std::printf("0121a6ac_TEST FAIL\n"); return 1; }
    if (n == 0 || n->Next != n || n->Prev != n || !NUISystem::CList::GPreviousSelected.empty()) { std::printf("0121a6ac_TEST FAIL\n"); return 1; }
    std::printf("0121a6ac_TEST PASS\n");
    return 0;
}