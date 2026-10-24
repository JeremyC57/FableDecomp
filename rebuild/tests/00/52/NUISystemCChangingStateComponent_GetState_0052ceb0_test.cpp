#include <cstdio>
struct CUIState { unsigned long m_word0; };
struct SStateNode { SStateNode* next; unsigned long first; CUIState second; };
struct CStateMap {
    struct iterator {
        SStateNode* _M_node;
        iterator(SStateNode* n) : _M_node(n) {}
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
        SStateNode* operator->() const { return _M_node; }
    };
    SStateNode* _M_header;                         /* sentinel; nodes chained from it */
    iterator find(const unsigned long& key) const  /* stand-in for the out-of-line tree find */
    {
        for (SStateNode* n = _M_header->next; n != _M_header; n = n->next)
            if (n->first == key) return iterator(n);
        return iterator(_M_header);
    }
    iterator end() const { return iterator(_M_header); }
};
struct CChangingStateComponent {
    CStateMap States;
    unsigned long CurrentState;
    CUIState* GetState(unsigned long index) { CStateMap::iterator it = States.find(index); if (it != States.end()) return &it->second; return 0; }
};
CUIState* Get(CChangingStateComponent& c, unsigned long k) { return c.GetState(k); }
int main()
{
    SStateNode head, a, b;
    head.next = &a; a.next = &b; b.next = &head;
    a.first = 1; a.second.m_word0 = 11; b.first = 4; b.second.m_word0 = 44;
    CChangingStateComponent c;
    c.States._M_header = &head; c.CurrentState = 0;
    if (Get(c, 1) != &a.second || Get(c, 4) != &b.second || Get(c, 2) != 0 || Get(c, 4)->m_word0 != 44)
        { std::printf("0052ceb0_TEST FAIL\n"); return 1; }
    std::printf("0052ceb0_TEST PASS\n");
    return 0;
}