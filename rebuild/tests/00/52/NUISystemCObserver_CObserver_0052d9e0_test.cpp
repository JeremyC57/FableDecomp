#include <cstdio>
#include <stdlib.h>
namespace std {
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
    void _M_empty_initialize()
    {
        this->_M_header->_M_color = false;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
};
template <class K, class C = less<K>, class A = allocator<K> > class set {
public:
    _Rb_tree<K, K, _Identity<K>, C, A> _M_t;
};
}
namespace NUISystem {
enum EEvent { EVENT_NONE };
class CObserver {
public:
    CObserver();
    virtual ~CObserver();
    std::set<EEvent> EventsToObserve;                    // +0x04
    bool             PreventObservation;                 // +0x10
};
CObserver::CObserver()
    : PreventObservation(false)
{
}
}

NUISystem::CObserver::~CObserver() {}
int main()
{
    NUISystem::CObserver* o = new NUISystem::CObserver;
    std::_Rb_tree_node_base* h = o->EventsToObserve._M_t._M_header;
    bool ok = h && !h->_M_color && h->_M_parent == 0 && h->_M_left == h && h->_M_right == h
        && o->EventsToObserve._M_t._M_node_count == 0 && !o->PreventObservation;
    if (ok) std::printf("0052d9e0_TEST PASS\n");
    return 0;
}