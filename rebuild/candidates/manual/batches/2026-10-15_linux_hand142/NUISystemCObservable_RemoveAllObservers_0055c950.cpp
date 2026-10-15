// NUISystem::CObservable::RemoveAllObservers() -- empties the observer list (+0x04, its count +0x08 reset) and the second observer list (+0x0c)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055C950, length 98 bytes (padded to 112).
// Label correction: the catalog "CGraphicBankManager::ProcessLoading" is propagated; the body is slot 6 of the
// NUISystem::CObservable vtable 0x01230044 (rtti-vtables.tsv) among the other CObservable slots (0x0055C930..0x0055CB10).
// The method name is descriptive.
// Callees: free @ 0x00BFEA14 (each list node).
// Shape: two inlined STLport `list::clear()` loops over trivially destructible elements (`tmp = cur; cur = cur->next;
// free(tmp)`, the sentinel member re-read after each free), each followed by the sentinel self-links; the count reset
// sits between the two clears.
// Leaf: ?RemoveAllObservers@CObservable@NUISystem@@
extern "C" void free(void* p);                     // 0x00BFEA14
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
struct list {                                      // STLport list of observer pointers: the sentinel node pointer
    _List_node_base* _M_node;
    void clear()
    {
        _List_node_base* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _List_node_base* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
namespace NUISystem {
struct CObservable {
    virtual void v00();
    list m_observers;                              // +0x04
    long m_observerCount;                          // +0x08
    list m_pendingObservers;                       // +0x0c
    void RemoveAllObservers();
};

void CObservable::RemoveAllObservers()
{
    m_observers.clear();
    m_observerCount = 0;
    m_pendingObservers.clear();
}
}
