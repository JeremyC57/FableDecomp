// _STL::copy(_Rb_tree_iterator<unsigned long> first, _Rb_tree_iterator<unsigned long> last, back_insert_iterator<vector<unsigned long> > result) -- STLport copy of a set<unsigned long> range into a vector through a back inserter: forwards to __copy_aux with a __false_type tag and returns the advanced inserter
// Leaf: copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00661287, 30 bytes, `ret 0Ch` (__fastcall: hidden result in ecx).
// Callee: 0x0066124B _STL::__copy_aux(first, last, result, const __false_type&) (__fastcall: hidden result in ecx, tag
// in edx, `ret 0Ch`; landed as `ConstructString`), which in turn calls __copy(..., const input_iterator_tag&,
// (ptrdiff_t*)0) @ 0x006611F7 -- that loop pushes each node value (+0x10) through vector::push_back 0x00485FF3 and
// steps with _Rb_tree_increment 0x004292C0.
// Identity: the manifest's `CPersistTraits<C2DVector>::TransferIn` is propagated. The only caller (0x0065CEE9, CTCVillage
// persistence) reserves a local vector<unsigned long>, calls this with (tree->leftmost, tree header, &vector) into a
// result slot, and hands the vector to CPersistContext::TransferVector<unsigned long> 0x005815D6.
// Shape: the empty tag argument is a 1-byte temporary VC homes in a dead argument slot (`lea edx,[ebp+13h]`, the top
// byte of the `result` slot), so the tag class has a user constructor (value-initialisation would store a zero byte);
// the iterators are one-word classes pushed by value (implicit copy); the inserter has STLport's user constructor, which
// makes the by-value result go through the hidden pointer (an aggregate would come back in eax); the hidden result
// pointer is parked in esi and returned in eax.
// EBP frame kept as in the rest of this module (optimize("y", off)).
#pragma optimize("y", off)
#pragma optimize("s", on)

namespace _STL {

struct __false_type { __false_type() { } };

struct _Rb_tree_node_base;
template <class T> struct _Rb_tree_iterator {           // one node pointer
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
};

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};

template <class C> struct back_insert_iterator {
    C* container;
    explicit back_insert_iterator(C& x) : container(&x) { }
};

typedef _Rb_tree_iterator<unsigned long> TSetIterator;
typedef back_insert_iterator<vector<unsigned long> > TInserter;

TInserter __fastcall __copy_aux(TSetIterator first, TSetIterator last, TInserter result, const __false_type& tag);   // 0x0066124B

TInserter __fastcall copy(TSetIterator first, TSetIterator last, TInserter result)
{
    return __copy_aux(first, last, result, __false_type());
}

}