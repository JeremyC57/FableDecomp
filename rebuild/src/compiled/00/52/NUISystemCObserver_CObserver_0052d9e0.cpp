// NUISystem::CObserver::CObserver() -- empty event set, observation not prevented
// Leaf: ??0CObserver@NUISystem@@QAE@XZ
// Retail: Fable.exe @ 0x0052D9E0, 59 bytes, `ret`; callers 0x005334B0, 0x00533730, 0x005953E7; Ego_r.pdb
// ??0CObserver@NUISystem@@QAE@XZ. The vtable stored (0x01246020) has RTTI .?AVCObserver@NUISystem@@ (no bases).
// Catalog label CFrontEndManager::Initialize is wrong.
// Body: vptr; STLport set<EEvent> EventsToObserve: _Rb_tree_base header = 0, then header = malloc(0x14) (0x00BFEA0E),
// node count 0, _M_empty_initialize (colour red, root 0, leftmost = rightmost = header); PreventObservation = false.
// Layout: retail STLport set is 12 bytes (header +4, count +8, compare +0xc), so PreventObservation is at +0x10;
// engine/CObserver.h carries the Ego_r.pdb 16-byte set and +0x14, so the class is modelled locally.
// Opt: /O2 (16-aligned entry; the size TU leaves the set constructor out of line).
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