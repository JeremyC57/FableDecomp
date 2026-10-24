// NUISystem::CObserver::IsObservingEvent(EEvent event) -- false while observation is prevented, else whether the event is in the observed set
// Leaf: IsObservingEvent
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052D900, 57 bytes (`ret 4`).
// Callee: STLport `__find(first, last, value, input_iterator_tag)` over set<EEvent> iterators @ 0x0052DEC0 (manifest
// "BSTIterator_FindNextNodeWithValue"; the landed front end 0x0052E0A0 forwards to it).
// Label: the catalog "NUISystem::CSprite::vfunc_2" is CSprite's CObserver-subobject vtable 0x0122F5AC slot 2; the CObserver
// vtable 0x01246020 holds the same body at slot 2. Ego_r.pdb names ?IsObservingEvent@CObserver@NUISystem@@UAE_NW4EEvent@2@@Z.
// Layout: Ego_r.pdb CObserver (0x18): EventsToObserve +0x04 (set<EEvent>, retail 12 bytes), PreventObservation +0x14 = retail +0x10
// (as the landed ObserveEvent 0x0052DA20 / SetPreventObserving 0x0041C580).
// Shape: the early `return false` keeps its own `xor al,al` exit; `std::find` (not set::find) over begin() = header->left and
// end() = header, the result iterator in the `push ecx` frame word, the tag temporary over the `event` slot; the compare with
// the cached header is a full-register `setne`.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

namespace std {
struct input_iterator_tag {
    input_iterator_tag() {}
};
struct _Rb_tree_node_base {
    unsigned char       _M_color;                  // +0x00
    _Rb_tree_node_base* _M_parent;                 // +0x04
    _Rb_tree_node_base* _M_left;                   // +0x08
    _Rb_tree_node_base* _M_right;                  // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                              // +0x10
};
_Rb_tree_node_base* __cdecl _Rb_tree_rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root,
    _Rb_tree_node_base*& leftmost, _Rb_tree_node_base*& rightmost);   // 0x0042A34E
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Rb_tree_node_base* n) : _M_node(n) {}
    V& operator*() const { return ((_Rb_tree_node<V>*)_M_node)->_M_value_field; }
    void _M_increment()
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* y = _M_node->_M_parent;
            while (_M_node == y->_M_right) {
                _M_node = y;
                y = y->_M_parent;
            }
            if (_M_node->_M_right != y)
                _M_node = y;
        }
    }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
    bool operator==(const _Rb_tree_iterator& o) const { return _M_node == o._M_node; }
    bool operator!=(const _Rb_tree_iterator& o) const { return _M_node != o._M_node; }
};
template <class K> class set {                     // STLport set: header, count, comparator (12 bytes)
public:
    typedef _Rb_tree_iterator<K> iterator;
    _Rb_tree_node_base* _M_header;                 // +0x00
    unsigned long       _M_node_count;             // +0x04
    unsigned char       _M_key_compare;            // +0x08
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    void erase(iterator position)
    {
        _Rb_tree_node_base* y = _Rb_tree_rebalance_for_erase(position._M_node, _M_header->_M_parent,
                                                             _M_header->_M_left, _M_header->_M_right);
        if (y)
            free(y);
        --_M_node_count;
    }
};
// The out-of-line algorithm (0x0052DEC0, compiled /Gr: `__fastcall`, the iterator through the hidden pointer in ecx, the value
// reference in edx, both iterators and the tag on the stack). Its body is visible in the TU with the tag unnamed and unused,
// which lets VC7.1 home the empty tag temporary over the live `event` argument slot like retail (GOTCHAS, advisor b35).
#pragma auto_inline(off)
template <class I, class T> __declspec(noinline) I __fastcall __find(I first, I last, const T& value, const input_iterator_tag&)
{
    while (first != last && !(*first == value))
        ++first;
    return first;
}
#pragma auto_inline(on)
template <class I, class T> inline I find(I first, I last, const T& value)
{
    return __find(first, last, value, input_iterator_tag());
}
}
namespace NUISystem {
enum EEvent { EVENT_NONE };
class CObserver {
public:
    virtual ~CObserver();
    virtual void ProcessEvent(EEvent event) = 0;   // slot 1
    virtual bool IsObservingEvent(EEvent event);   // slot 2
    std::set<EEvent> EventsToObserve;              // +0x04
    bool             PreventObservation;           // +0x10
};

bool CObserver::IsObservingEvent(EEvent event)
{
    if (PreventObservation)
        return false;
    return std::find(EventsToObserve.begin(), EventsToObserve.end(), event) != EventsToObserve.end();
}
}