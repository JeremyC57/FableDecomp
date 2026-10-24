// std::__find(first, last, const CIntelligentPointer<const CThing>& val, const input_iterator_tag&) -- STLport linear search of a list of intelligent thing pointers for the node holding the same thing as val
// Leaf: __find
// Retail: Fable.exe @ 0x0069CAF0, 60 bytes, `ret 0Ch`: __fastcall with the hidden iterator result in ecx, val BY REFERENCE
// in edx (kept in ebx) and first / last / the unused category tag on the stack. Call sites 0x006984E5, 0x0069923C,
// 0x0069CF6C (each copy-constructs both iterators into their argument slots).
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (resolves the held object, null when it died).
// Shape: the value is an intelligent pointer, not a raw CThing* (the catalog's "CThing*" element): GetPItem runs on the
// node's pointer (`lea ecx,[esi+8]`) and then on val (`mov ecx,ebx`) every trip -- the engine operator== compares the
// two resolved objects, left operand first. List node: _M_next +0, _M_prev +4, data +8. `last` is re-read from its
// stack slot inside the loop (no register left: ebx val, edi result, esi node, ebp left object).
class CBaseObject;
class CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                       // +0x04
    CBaseObject* GetPItem() const;              // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
};
template <class T>
inline bool operator==(const CIntelligentPointer<T>& a, const CIntelligentPointer<T>& b) { return a.GetPItem() == b.GetPItem(); }

namespace std {
struct input_iterator_tag {};
template <class T> struct _Nonconst_traits {};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node == y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node != y._M_node; }
};
template <class T, class Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) {}
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) {}
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_incr(); return *this; }
};
template <class InputIter, class T>
InputIter __fastcall __find(InputIter first, InputIter last, const T& val, const input_iterator_tag&)
{
    while (first != last && !(*first == val))
        ++first;
    return first;
}
typedef CIntelligentPointer<const CThing> CThingPointer;
typedef _List_iterator<CThingPointer, _Nonconst_traits<CThingPointer> > _ThingListIter;
template _ThingListIter __fastcall __find(_ThingListIter, _ThingListIter, const CThingPointer&, const input_iterator_tag&);
}