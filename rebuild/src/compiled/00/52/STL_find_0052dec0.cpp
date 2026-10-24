// _STL::__find(_Rb_tree_iterator<EEvent> first, _Rb_tree_iterator<EEvent> last, const EEvent& value, const input_iterator_tag&) -- STLport input-iterator find over a set<NUISystem::EEvent>: step the tree iterator in order until it reaches last or a node whose value equals value; returns the iterator
// Leaf: ??$__find
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052DEC0, 95 bytes, `ret 0Ch` (/Gr `__fastcall`: the hidden iterator result in ecx,
//   the value reference in edx, first / last / the tag reference on the stack).
// Callers: NUISystem::CObserver::IsObservingEvent 0x0052D900 / IgnoreEvent 0x0052D940 (landed), the front end 0x0052E0A0,
//   NUISystem::CHoverable::IgnoreEvent 0x0055BD50 (landed).
// Label correction: catalogued as "BSTIterator_FindNextNodeWithValue" (propagated); it is the STLport `__find` instantiation
//   the landed CObserver rows already declare at this address.
// Layout: STLport tree nodes { _M_color +0, _M_parent +4, _M_left +8, _M_right +0xc, value +0x10 }; the iterator is the
//   one node pointer.
// Shape: STLport 4.x `while (__first != __last && !(*__first == __val)) ++__first; return __first;` with the inline
//   `_Rb_tree_iterator::_M_increment` (right subtree -> its leftmost node, else climb while we are a right child, then
//   `if (_M_node->_M_right != __y) _M_node = __y`) written as STLport's static `_Rb_global<bool>::_M_increment(node)`
//   returning the node (`_M_node = _M_increment(_M_node)`): a member `_M_increment()` on the iterator swaps the node /
//   value registers (esi/edx) and moves the loop-head alignment (92v95). The value is loaded once (`mov edi,[esi]`); the result is stored
//   through the hidden pointer at the single exit. The algorithm TU is compiled /Gr, so the template is `__fastcall`.
//   A caller (CObserver::IgnoreEvent, as landed at 0x0052D940) instantiates it.
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
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)   // _Rb_global<bool>::_M_increment, inline
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
        return _M_node;
    }
    _Rb_tree_iterator& operator++() { _M_node = _M_increment(_M_node); return *this; }
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
    virtual void ObserveEvent(EEvent event);       // slot 3
    virtual void IgnoreEvent(EEvent event);        // slot 4
    std::set<EEvent> EventsToObserve;              // +0x04
    bool             PreventObservation;           // +0x10
};

void CObserver::IgnoreEvent(EEvent event)          // 0x0052D940 (landed): the instantiating caller
{
    std::set<EEvent>::iterator it = std::find(EventsToObserve.begin(), EventsToObserve.end(), event);
    if (it != EventsToObserve.end())
        EventsToObserve.erase(it);
}
}