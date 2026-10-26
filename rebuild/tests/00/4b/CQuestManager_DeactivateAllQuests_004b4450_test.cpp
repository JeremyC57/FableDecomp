#include <cstdio>
class CCPPointerInfo { public:
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

static int g_calls = 0; static bool g_ok = true; static long g_seen_ref = 0;
template <class T> CCountedPointer<T>::~CCountedPointer() { if (Info) --Info->RefCount; }
void CQuestManager::DeactivateQuest(CCountedPointer<CActiveQuest> quest, unsigned long reason)
{
    ++g_calls; if (reason != 0) g_ok = false;
    g_seen_ref = quest.Info ? quest.Info->RefCount : -1;
    // unlink the front node
    std::_List_node_base* head = ActiveQuests._M_node; std::_List_node_base* first = head->_M_next;
    head->_M_next = first->_M_next; first->_M_next->_M_prev = head;
}
int main()
{
    static CQuestManager m; static std::_List_node_base head;
    static unsigned char raw[2][sizeof(std::_List_node<CCountedPointer<CQuestManager::CActiveQuest> >)];
    typedef std::_List_node<CCountedPointer<CQuestManager::CActiveQuest> > Node;
    Node* n0 = (Node*)raw[0]; Node* n1 = (Node*)raw[1];
    static CCPPointerInfo info = { 1 };
    n0->_M_data.Data = 0; n0->_M_data.Info = &info; n1->_M_data.Data = 0; n1->_M_data.Info = 0;
    head._M_next = n0; n0->_M_next = n1; n1->_M_next = &head; head._M_prev = n1; n1->_M_prev = n0; n0->_M_prev = &head;
    m.ActiveQuests._M_node = &head;
    m.DeactivateAllQuests();
    bool ok = g_ok && g_calls == 2 && head._M_next == &head && info.RefCount == 1 && g_seen_ref == -1;
    if (ok) std::printf("004b4450_TEST PASS\n");
    return 0;
}