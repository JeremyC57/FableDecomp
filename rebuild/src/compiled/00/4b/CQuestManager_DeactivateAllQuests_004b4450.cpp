// CQuestManager::DeactivateAllQuests() -- deactivate the front active quest until none are left
// Leaf: ?DeactivateAllQuests@CQuestManager@@QAEXXZ
// Retail: Fable.exe @ 0x004B4450, 61 bytes, `ret`; callers 0x004A228E, 0x004B4787, 0x004B49E3; Ego_r.pdb
// ?DeactivateAllQuests@CQuestManager@@QAEXXZ.
// Body: while ActiveQuests (STLport list, node pointer at +0x38) is not empty, DeactivateQuest(ActiveQuests.front(), 0)
// (0x004B3A20, Ego_r.pdb ?DeactivateQuest@CQuestManager@@AAEXV?$CCountedPointer@VCActiveQuest@CQuestManager@@@@K@Z): the
// counted pointer is copy-constructed by value onto the stack (Data, Info, ++Info->RefCount when non-null).
// Layout: retail ActiveQuests +0x38 (STLport 4-byte list); engine/CQuestManager.h carries the Ego_r.pdb +0x44 (VC7
// vector/list sizes), so the class is modelled locally. List node: next +0, prev +4, value +8.
// Shape: /O2 (16-aligned entry, `lea ebx,[ebx]` loop-head pad); front() as *begin() through the node-base iterator cast
// (a direct node-to-data reference adds an `add eax,8`).
struct CCPPointerInfo {
    long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer();
};
namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* x) : _M_node(x) {}
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
};
template <class T> class list {
public:
    _List_node_base* _M_node;
    _List_iterator<T> begin() { return _List_iterator<T>(_M_node->_M_next); }
    bool empty() const { return _M_node->_M_next == _M_node; }
    T& front() { return *begin(); }
};
}
class CQuestManager {
public:
    class CActiveQuest;
    unsigned char                                 _pad00[0x38];
    std::list<CCountedPointer<CActiveQuest> >     ActiveQuests;      // +0x38
    void DeactivateAllQuests();
private:
    void DeactivateQuest(CCountedPointer<CActiveQuest> quest, unsigned long reason);   // 0x004B3A20
    friend struct CQuestManager_Test;                    // behaviour-test hook
};
void CQuestManager::DeactivateAllQuests()
{
    while (!ActiveQuests.empty())
        DeactivateQuest(ActiveQuests.front(), 0);
}