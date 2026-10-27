// std::_List_iterator::operator++() -- advance a list iterator to the next node (prefix increment)
// Leaf: ??E?$_List_iterator@VCUnprovenValue_00685200@@U?$_Nonconst_traits@VCUnprovenValue_00685200@@@std@@@std@@QAEAAU01@XZ
// Retail: Fable.exe @ 0x00685200, 9 bytes: `mov eax,ecx ; mov ecx,[eax] ; mov edx,[ecx] ; mov [eax],edx ; ret`.
// Identity (descriptive element type): the STLport 4.6 prefix `_List_iterator::operator++` (`_M_incr()` then `return *this`):
// the node's next pointer is its first word, the iterator is the node pointer. The row is an unreferenced gap-scan COMDAT
// (every "abs" xref is a false hit inside other instructions' bytes) among the out-of-line STL pieces of the
// CTCCreatureOpinion area (0x00685130..0x00685700), so the element type is not provable: CUnprovenValue_00685200.
class CUnprovenValue_00685200 { public: long _value; };     // descriptive: element type unproven
namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp  value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    _List_iterator(_List_node<_Tp>* __x) : _List_iterator_base(__x) {}
    _Self& operator++() { this->_M_incr(); return *this; }
};
template struct _List_iterator<CUnprovenValue_00685200, _Nonconst_traits<CUnprovenValue_00685200> >;
}