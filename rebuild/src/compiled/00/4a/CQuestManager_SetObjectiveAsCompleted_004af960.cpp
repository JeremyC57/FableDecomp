// CQuestManager::SetObjectiveAsCompleted(unsigned long) -- mark the objective with this index as completed
// Leaf: ?SetObjectiveAsCompleted@CQuestManager@@QAEXK@Z
// Retail: Fable.exe @ 0x004AF960, 40 bytes; Ego_r.pdb same symbol (next to SetObjectiveAsFailed). Walks the STLport
// list Objectives (sentinel pointer +0x74; node next +0, value +8) and on the first CObjective whose Index (+4) matches
// stores CompletionStatus (+8) = 1, then returns. Reached by a tail jump from 0x00891806 (the script interface).
// Layout: Ego_r.pdb CObjective {CWideString Name, ulong Index, ECompletionStatus CompletionStatus}; CQuestManager
// Objectives at PDB +0xc8, retail +0x74 (STLport lists are 4 bytes, vectors 12). The enumerator name is behavioural.
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