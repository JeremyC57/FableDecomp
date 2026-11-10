// std::operator==(const reverse_iterator<CUnprovenIterator_0083ADF0>& __x, const reverse_iterator<CUnprovenIterator_0083ADF0>& __y) -- two reverse iterators are equal when their base iterators are
// Leaf: ??$?8UCUnprovenIterator_0083ADF0@@@std@@YA_NABV?$reverse_iterator@UCUnprovenIterator_0083ADF0@@@0@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083ADF0, 22 bytes, cdecl: mov eax,[esp+8] ; mov ecx,[esp+4] ; mov eax,[eax] ; mov ecx,[ecx] ;
//   xor edx,edx ; cmp ecx,eax ; sete dl ; mov al,dl ; ret.
// Identity (probable member, unproven instantiation): a free `_STLP_CALL` (= __cdecl) bool of two references that compares
//   COPIES of their first words: STLport 4.6 stl/_iterator.h has exactly one free template of that shape,
//   `operator==(const reverse_iterator<_It>&, const reverse_iterator<_It>&) { return __x.base() == __y.base(); }` (base() returns
//   the iterator by value). Measured (agent897 probes): the `xor/cmp/sete dl/mov al,dl` body needs the base iterator to be a
//   one-pointer POD compared by a member operator== (a raw pointer, a direct compare of the references, or an iterator class
//   with user constructors such as _Rb_tree_iterator all give the 18-byte `sub/neg/sbb/inc` form), so the inner iterator type
//   stays the placeholder CUnprovenIterator_0083ADF0 (a POD in the _Rb_tree_base_iterator shape). Unreferenced in retail (no
//   call, jump or absolute reference): a COMDAT the TU of the landed tree find 0x0083B120 kept (no /OPT:REF).
// Shape: explicit instantiation emits the inline template as its own COMDAT.
struct CUnprovenNode_0083ADF0;
struct CUnprovenIterator_0083ADF0 {                          // a one-pointer POD iterator with a member equality
    CUnprovenNode_0083ADF0* _M_node;
    bool operator==(const CUnprovenIterator_0083ADF0& __y) const { return _M_node == __y._M_node; }
};
namespace std {
template <class _Iterator> class reverse_iterator {          // stl/_iterator.h (the parts this row uses)
protected:
    _Iterator current;
public:
    typedef _Iterator iterator_type;
    reverse_iterator() { }
    explicit reverse_iterator(iterator_type __x) : current(__x) { }
    iterator_type base() const { return current; }
};
template <class _Iterator>
inline bool __cdecl operator==(const reverse_iterator<_Iterator>& __x, const reverse_iterator<_Iterator>& __y)
{
    return __x.base() == __y.base();
}
}

template bool __cdecl std::operator==(const std::reverse_iterator<CUnprovenIterator_0083ADF0>& __x,
                                     const std::reverse_iterator<CUnprovenIterator_0083ADF0>& __y);