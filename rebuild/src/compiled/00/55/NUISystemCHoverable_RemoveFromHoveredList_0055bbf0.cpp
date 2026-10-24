// NUISystem::CHoverable::RemoveFromHoveredList() -- take this component out of the shared hovered-component list
// Leaf: RemoveFromHoveredList
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055BBF0, 52 bytes (plain `ret`).
// Callee: free @ 0x00BFEA14 (the list node).
// Label: the catalog "NUISystem::CMenuEntry::vfunc_133" is the first vtable listing this body; the CHoverable vtable
// 0x0124BFB4 holds it at slot 133 and Ego_r.pdb names ?RemoveFromHoveredList@CHoverable@NUISystem@@UAEXXZ (CHoverable::
// IgnoreEvent 0x0055BD50 calls it through slot 133).
// Layout: the static list<CComponent*> of hovered components at 0x013B8AD4 (STLport, the sentinel pointer; read by the
// hover code 0x0055A6CD..0x0055C0ED); the name is descriptive.
// Shape: STLport `std::find(begin, end, this)` inlined (`this` stays in ecx, the loop aligned with `lea esp,[esp]`); the
// not-found loop exit returns at once, the found arm re-tests against end and erases the node (unlink, free).
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);        // 0x00BFEA14
inline void* operator new(size_t, void* where) { return where; }

namespace NUISystem {
enum EEvent { EVENT_NONE };
class CObserver {                                  // vtable 0x01246020
public:
    virtual ~CObserver();                          // slot 0
    virtual void ProcessEvent(EEvent event) = 0;   // slot 1 (+0x04)
    virtual bool IsObservingEvent(EEvent event);   // slot 2 (+0x08), 0x0052D900
    virtual void ObserveEvent(EEvent event);       // slot 3 (+0x0c), 0x0052DA20
    virtual void IgnoreEvent(EEvent event);        // slot 4 (+0x10), 0x0052D940
    virtual void IgnoreAllEvents();                // slot 5 (+0x14), 0x0052D9A0
};
}

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template <class T> class list {                    // STLport list: the 4-byte sentinel pointer
public:
    typedef _List_node<T> _Node;
    struct iterator {
        _List_node_base* _M_node;
        iterator(_List_node_base* n) : _M_node(n) {}
        T& operator*() const { return ((_Node*)_M_node)->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator==(const iterator& o) const { return _M_node == o._M_node; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    _List_node_base* _M_node;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    _Node* _M_create_node(const T& x)
    {
        _Node* p = (_Node*)malloc(sizeof(_Node));
        new (&p->_M_data) T(x);
        return p;
    }
    iterator insert(iterator position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    void push_back(const T& x) { insert(end(), x); }
    iterator erase(iterator position)
    {
        _List_node_base* next = position._M_node->_M_next;
        _List_node_base* prev = position._M_node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(position._M_node);
        return iterator(next);
    }
};
template <class I, class T> inline I find(I first, I last, const T& value)
{
    while (first != last && !(*first == value))
        ++first;
    return first;
}
}
namespace NUISystem {
class CComponent;
class CHoverable {
public:
    static std::list<CComponent*> GHoveredComponents;   // 0x013B8AD4
    CComponent* AsComponent() { return (CComponent*)this; }
    void RemoveFromHoveredList();
};

void CHoverable::RemoveFromHoveredList()
{
    std::list<CComponent*>::iterator it = std::find(GHoveredComponents.begin(), GHoveredComponents.end(), AsComponent());
    if (it != GHoveredComponents.end())
        GHoveredComponents.erase(it);
}
}