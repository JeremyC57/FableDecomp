// NUISystem::CObserver::ObserveEvent(EEvent event) -- add an event to the observed set; the first time event 25 is observed, notify through the pure handler
// Leaf: ?ObserveEvent@CObserver@NUISystem@@UAEXW4EEvent@2@@Z
// Retail: Fable.exe @ 0x0052DA20, 78 bytes, `ret 4`; CObserver vtable slot 3 (0x01246020 +0x0c) and 30 derived vtables;
// Ego_r.pdb ?ObserveEvent@CObserver@NUISystem@@UAEXW4EEvent@2@@Z.
// Body: it = EventsToObserve.find(event) (0x0052DF20), end() read, EventsToObserve.insert(event) (0x0052E230, result
// unused); if (event == 25 && it == end) the virtual slot 1 (+0x04, pure in CObserver -- `_purecall` 0x00BFEA08 in its
// vtable; name not recovered) with 25.
// Layout: retail STLport set<EEvent> EventsToObserve +0x04 (12 bytes), as 0x0052D9E0.
// Shape: /O2 (16-aligned entry); end() bound to a local before the insert, iterators with a user copy constructor.
#include <stdlib.h>
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() {}
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
template <class T> struct less {};
template <class T> class allocator {};
template <class V> struct _Identity {};
struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00 (_Rb_tree_Color_type, red = false)
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
template <class V, class A> class _Rb_tree_base {
public:
    _Rb_tree_node<V>* _M_header;
    _Rb_tree_base() : _M_header(0) { _M_header = (_Rb_tree_node<V>*)malloc(sizeof(_Rb_tree_node<V>)); }
};
template <class K, class V, class KoV, class C, class A> class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    size_t _M_node_count;
    C      _M_key_compare;
    _Rb_tree() : _M_node_count(0) { _M_empty_initialize(); }
    private:
    void _M_empty_initialize()
    {
        this->_M_header->_M_color = false;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
    public:
};
template <class K, class C = less<K>, class A = allocator<K> > class set {
public:
    typedef _Rb_tree<K, K, _Identity<K>, C, A> _Rep_type;
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator() {}
        iterator(const iterator& o) : _M_node(o._M_node) {}
        bool operator==(const iterator& o) const { return _M_node == o._M_node; }
    };
    _Rep_type _M_t;
    iterator find(const K& key);                         // 0x0052DF20 (out of line, result through a hidden pointer)
    pair<iterator, bool> insert(const K& key);           // 0x0052E230
    iterator end() { iterator it; it._M_node = _M_t._M_header; return it; }
};
}
namespace NUISystem {
enum EEvent { EVENT_NONE, EVENT_25 = 25 };
class CObserver {
public:
    virtual ~CObserver();
    virtual void HandleEvent(EEvent event) = 0;          // slot 1 (+0x04)
    virtual void ObserveAllEvents();                     // slot 2
    virtual void ObserveEvent(EEvent event);             // slot 3
    std::set<EEvent> EventsToObserve;                    // +0x04
    bool             PreventObservation;                 // +0x10
};
void CObserver::ObserveEvent(EEvent event)
{
    std::set<EEvent>::iterator it = EventsToObserve.find(event);
    std::set<EEvent>::iterator end = EventsToObserve.end();
    EventsToObserve.insert(event);
    if (event == EVENT_25 && it == end)
        HandleEvent(EVENT_25);
}
}