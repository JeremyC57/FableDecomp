// NUISystem::CObservable::IsExclusivelyObserved() -- true when an exclusive observer is set or any concurrent exclusive observer is listed
// Leaf: IsExclusivelyObserved
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055CAE0, 35 bytes (plain `ret`), no callees.
// Label: CObservable vtable 0x01230044 slot 8 (rtti-vtables.tsv); Ego_r.pdb names
// ?IsExclusivelyObserved@CObservable@NUISystem@@UAE_NXZ.
// Layout: m_pExclusiveObserver +0x08, ConcurrentExclusiveObservers +0x0c (STLport list, 4-byte sentinel pointer).
// Shape: STLport `list::size()` walks the nodes (`distance` into a zeroed counter); two sequential `if (..) return true;` tests share the `mov al,1`
// tail placed before the `xor al,al` one (`if (a || b) return true;` and `if (!a && !b) return false;` put the false tail
// first, `return a || b;` materialises `mov eax,1` / `xor eax,eax`).
namespace NUISystem {
class CObserver;
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
class CObserverList {                              // STLport list<CObserver*>
public:
    _List_node_base* _M_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        for (_List_node_base* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
};
class CObservable {                                // vtable 0x01230044
public:
    virtual void v000();
    CObserverList Observers;                       // +0x04
    CObserver*    m_pExclusiveObserver;            // +0x08
    CObserverList ConcurrentExclusiveObservers;    // +0x0c
    bool IsExclusivelyObserved();
};

bool CObservable::IsExclusivelyObserved()
{
    if (m_pExclusiveObserver)
        return true;
    if (ConcurrentExclusiveObservers.size() != 0)
        return true;
    return false;
}
}