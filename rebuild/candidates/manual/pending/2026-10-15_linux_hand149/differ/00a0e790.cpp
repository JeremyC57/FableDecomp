// std::__copy_aux(_Deque_iterator<C2DVector>, _Deque_iterator<C2DVector>, _Deque_iterator<C2DVector>, const __false_type&) -- STLport copy() step for deque iterators: forward to the random-access __copy with the iterator category and a null distance pointer
// Grade: DIFFER(120v118) -- the empty random_access_iterator_tag temporary passed by reference in edx: retail homes it in the dead slot of the by-value `result` argument (`lea edx,[esp+5Ch]`, no frame), ours opens a `push ecx` frame word for it (`lea edx,[esp+3Bh]`, every stack offset +4, `pop ecx`). Tried: tag with/without user ctor, STLport tag inheritance chain, iterator_category()/distance_type() helpers.
// Leaf: __copy_aux
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0E790, 118 bytes (`ret 30h`).
// Label correction: the catalog calls this CNavigatorAStar::MoveToNavLineOfSightProximity (a propagated name; that PDB signature
//   has 12 arguments and a thiscall `this`). The body is a `__fastcall` template: ecx = the hidden result iterator (returned in
//   eax), edx = the unused `const __false_type&`, three 16-byte deque iterators {cur, first, last, node} by value (0x30 bytes).
//   It is called from the copy() forwarder at 0x00A0EB40 and calls __copy (0x00A0E460, label "DequeIterator_CopyForward"),
//   whose distance arithmetic (`sar 3`, 16 elements per node) gives 8-byte elements: deque<C2DVector> in this module.
// Shape: `__copy(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` -- the empty tag temporary lives in the
//   dead slot of `result` and goes in edx by reference; the three iterator copies are built in the argument area right to left.
struct C2DVector {
    float X, Y;
};
namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { };
template <class T> struct _Deque_iterator {        // STLport deque iterator, 16 bytes
    T*  _M_cur;
    T*  _M_first;
    T*  _M_last;
    T** _M_node;
    _Deque_iterator() { }
    _Deque_iterator(const _Deque_iterator& other) : _M_cur(other._M_cur), _M_first(other._M_first), _M_last(other._M_last), _M_node(other._M_node) { }
};
template <class T> _Deque_iterator<T> __fastcall __copy(_Deque_iterator<T> first, _Deque_iterator<T> last, _Deque_iterator<T> result,
                                                       const random_access_iterator_tag& tag, int* distance);   // 0x00A0E460
template <class T> _Deque_iterator<T> __fastcall __copy_aux(_Deque_iterator<T> first, _Deque_iterator<T> last, _Deque_iterator<T> result,
                                                           const __false_type& trivial)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}
template _Deque_iterator<C2DVector> __fastcall __copy_aux(_Deque_iterator<C2DVector>, _Deque_iterator<C2DVector>, _Deque_iterator<C2DVector>, const __false_type&);
}
