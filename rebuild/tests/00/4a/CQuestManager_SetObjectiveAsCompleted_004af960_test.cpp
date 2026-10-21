// test for 004af960 CQuestManager::SetObjectiveAsCompleted
#include <stdio.h>
#include <string.h>
class CWideString { public: void* PData; };
namespace std {
template <class T> struct _List_node {
    _List_node* _M_next;                                 // +0x00
    _List_node* _M_prev;                                 // +0x04
    T           _M_data;                                 // +0x08
};
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* n) : _M_node(n) {}
    T& operator*() const { return _M_node->_M_data; }
    T* operator->() const { return &_M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& x) const { return _M_node != x._M_node; }
};
template <class T> class list {
public:
    typedef _List_iterator<T> iterator;
    _List_node<T>* _M_node;                              // sentinel
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
};
}
class CQuestManager {
public:
    enum ECompletionStatus { COMPLETION_STATUS_NONE, COMPLETION_STATUS_COMPLETED, COMPLETION_STATUS_FAILED };
    class CObjective {
    public:
        CWideString       Name;                          // +0x00
        unsigned long     Index;                         // +0x04
        ECompletionStatus CompletionStatus;              // +0x08
    };
    void SetObjectiveAsCompleted(unsigned long index);
    unsigned char          _pad00[0x74];
    std::list<CObjective>  Objectives;                   // +0x74
};
void CQuestManager::SetObjectiveAsCompleted(unsigned long index)
{
    for (std::list<CObjective>::iterator it = Objectives.begin(); it != Objectives.end(); ++it) {
        if (it->Index == index) {
            it->CompletionStatus = COMPLETION_STATUS_COMPLETED;
            return;
        }
    }
}

int main()
{
    typedef std::_List_node<CQuestManager::CObjective> N;
    static N sent, a, b, c;
    sent._M_next = &a; a._M_next = &b; b._M_next = &c; c._M_next = &sent;
    a._M_data.Index = 3; b._M_data.Index = 5; c._M_data.Index = 5;
    a._M_data.CompletionStatus = b._M_data.CompletionStatus = c._M_data.CompletionStatus = CQuestManager::COMPLETION_STATUS_NONE;
    static double buf[0x20]; CQuestManager* q = (CQuestManager*)buf; q->Objectives._M_node = &sent;
    q->SetObjectiveAsCompleted(5);
    int ok = a._M_data.CompletionStatus == 0 && b._M_data.CompletionStatus == 1 && c._M_data.CompletionStatus == 0;
    q->SetObjectiveAsCompleted(9);
    ok &= a._M_data.CompletionStatus == 0 && (char*)&q->Objectives - (char*)q == 0x74;
    printf(ok ? "004af960_TEST PASS\n" : "004af960_TEST FAIL\n");
    return ok ? 0 : 1;
}