#include <cstdio>
// Behaviour of CQuestManager::IsQuestCardActive (0x004AF560): true as soon as a live active quest card's def
// name equals the requested name; dead (null) cards are skipped; false when none matches.
struct CCharString { long m_id; };
struct CDefString {
    long m_index;
    CDefString() : m_index(0) {}
    bool operator==(const CCharString& name) const { return m_index == name.m_id; }
};
struct CThing {
    long m_defIndex;
    CDefString GetDefName() const { CDefString d; d.m_index = m_defIndex; return d; }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
struct CIntelligentPointer : CBaseIntelligentPointer { CThing* Get() const { return GetPItem(); } };
template <class T> struct SListNode { SListNode* _M_next; SListNode* _M_prev; T _M_data; };
template <class T> struct CRetailList {
    struct iterator {
        SListNode<T>* _M_node;
        iterator(SListNode<T>* n) : _M_node(n) {}
        T& operator*() const { return _M_node->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    SListNode<T>* _M_node;
    iterator begin() const { return iterator(_M_node->_M_next); }
    iterator end() const { return iterator(_M_node); }
};
struct CQuestManager {
    CRetailList<CIntelligentPointer> ActiveQuestCards;
    bool IsQuestCardActive(const CCharString& defName)
    {
        for (CRetailList<CIntelligentPointer>::iterator it = ActiveQuestCards.begin(); it != ActiveQuestCards.end(); ++it) {
            CThing* card = (*it).Get();
            if (card && card->GetDefName() == defName)
                return true;
        }
        return false;
    }
};
int main()
{
    CThing a, b; a.m_defIndex = 10; b.m_defIndex = 20;
    SListNode<CIntelligentPointer> head, n1, n2, n3;
    head._M_next = &n1; n1._M_next = &n2; n2._M_next = &n3; n3._M_next = &head;
    n1._M_data.m_pItem = &a; n2._M_data.m_pItem = 0; n3._M_data.m_pItem = &b;
    CQuestManager q; q.ActiveQuestCards._M_node = &head;
    CCharString n20; n20.m_id = 20; CCharString n10; n10.m_id = 10; CCharString n0; n0.m_id = 0; CCharString n30; n30.m_id = 30;
    if (!q.IsQuestCardActive(n20) || !q.IsQuestCardActive(n10) || q.IsQuestCardActive(n0) || q.IsQuestCardActive(n30)) { std::printf("004af560_TEST FAIL\n"); return 1; }
    std::printf("004af560_TEST PASS\n");
    return 0;
}
