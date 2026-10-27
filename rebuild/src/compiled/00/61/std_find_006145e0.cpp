// std::__find(_List_iterator first, _List_iterator last, const CUnprovenPair8_006145e0& value, const input_iterator_tag&) -- STLport linear search over a list: advance first until it reaches last or an element equal (both words) to value
// Leaf: ??$__find@U?$_List_iterator@UCUnprovenPair8_006145e0@@U?$_Nonconst_traits@UCUnprovenPair8_006145e0@@@std@@@std@@UCUnprovenPair8_006145e0@@@std@@YI?AU?$_List_iterator@UCUnprovenPair8_006145e0@@U?$_Nonconst_traits@UCUnprovenPair8_006145e0@@@std@@@0@U10@0ABUCUnprovenPair8_006145e0@@ABUinput_iterator_tag@0@@Z
// Retail: Fable.exe @ 0x006145E0, 52 bytes, `__fastcall` (hidden iterator result in ecx, &value in edx; first, last and
// the tag reference on the stack), `ret 0Ch`.
// Identity (descriptive element type): STLport 4.6 `__find(_InputIter, _InputIter, const _Tp&, const input_iterator_tag&)`
// (`while (__first != __last && !(*__first == __val)) ++__first; return __first;`) over list iterators (node data at +8,
// next at +0). Its only caller is the std::find wrapper 0x006157F0 (`__find(first, last, val, _STLP_ITERATOR_CATEGORY)`,
// itself unreferenced), so the 8-byte element (an equality over two words, e.g. a pair of longs) cannot be named:
// CUnprovenPair8_006145e0.
// Callees: none.
// Shape: the empty-range test before the loop with its own `mov [eax],esi` exit; the value's first word hoisted into edi,
// the second compared from memory; the iterator returned through the hidden pointer.
struct CUnprovenPair8_006145e0 {                            // 8-byte list element (unproven)
    long First;                                             // +0x00
    long Second;                                            // +0x04
};
inline bool operator==(const CUnprovenPair8_006145e0& x, const CUnprovenPair8_006145e0& y)
{
    return x.First == y.First && x.Second == y.Second;
}

namespace std {
struct input_iterator_tag { };

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x00
    _List_node_base* _M_prev;                               // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x08
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef typename _Traits::reference reference;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _Self& __x) : _List_iterator_base(__x._M_node) { }
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
};

template <class _InputIter, class _Tp>
_InputIter __fastcall __find(_InputIter __first, _InputIter __last, const _Tp& __val, const input_iterator_tag&)
{
    while (__first != __last && !(*__first == __val))
        ++__first;
    return __first;
}

typedef _List_iterator<CUnprovenPair8_006145e0, _Nonconst_traits<CUnprovenPair8_006145e0> > TIter_006145e0;
template TIter_006145e0 __fastcall __find(TIter_006145e0, TIter_006145e0, const CUnprovenPair8_006145e0&, const input_iterator_tag&);
}