// std::copy(_Deque_iterator<C2DVector> first, _Deque_iterator<C2DVector> last, _Deque_iterator<C2DVector> result) -- STLport copy() over deque iterators: forward to __copy_aux with the not-both-pointers __false_type tag
// Leaf: copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0EB40, length 116 bytes (+ int3 padding), `ret 30h`.
// Callees: 0x00A0E790 std::__copy_aux(first, last, result, const __false_type&) (itself forwarding to the random-access __copy 0x00A0E460; deque<C2DVector> by its 8-byte elements).
// Label correction: the catalog calls this CEnginePrimitiveRenderer::AddStaticMesh (propagated across the three
// byte-identical instantiations 0x00A0E680 / 0x00A0EB40 / 0x00A0EF90, each forwarding to its own callee); the body is
// a free STLport template function of the algorithm TU.
// Shape: the algorithm TU is compiled with /Gr, so the function is `__fastcall`: the 16-byte iterator result comes
// back through the hidden pointer in ecx (kept in esi and returned in eax), the three iterators arrive by value on the
// stack (`ret 30h`). Each iterator has a user copy constructor, so every by-value argument of the forwarded call is
// built in its reserved slot (`sub esp,10h ; mov eax,esp ; mov [eax],..`); the empty `__false_type` tag (user
// constructor: no zero store) is passed by const reference in edx and homed in the dead `result` argument slot
// (`lea edx,[esp+58h]`). That sharing needs a same-TU noinline stand-in for the callee, the const-ref taker of the
// tag; extern, VC7.1 opens a `push ecx` frame word for it (the "tag over a dead argument slot" lane gap was this escape).
struct C2DVector { float X, Y; };
namespace std {
struct __false_type { __false_type() { } };
template <class T> struct _Deque_iterator {        // STLport deque iterator, 16 bytes
    T*  _M_cur;
    T*  _M_first;
    T*  _M_last;
    T** _M_node;
    _Deque_iterator(const _Deque_iterator& other) : _M_cur(other._M_cur), _M_first(other._M_first), _M_last(other._M_last), _M_node(other._M_node) { }
};
// Same-TU stand-in for 0x00A0E790 (not the retail body).
void CopyAux_00a0e790(void* out, const void* first, const void* last, const void* result);
#pragma auto_inline(off)
template <class T> __declspec(noinline) _Deque_iterator<T> __fastcall __copy_aux(_Deque_iterator<T> first, _Deque_iterator<T> last,
                                                                     _Deque_iterator<T> result, const __false_type& tag)
{
    CopyAux_00a0e790(&result, &first, &last, &result);
    return result;
}
#pragma auto_inline(on)
template <class T> _Deque_iterator<T> __fastcall copy(_Deque_iterator<T> first, _Deque_iterator<T> last, _Deque_iterator<T> result)
{
    return __copy_aux(first, last, result, __false_type());
}
template _Deque_iterator<C2DVector> __fastcall copy(_Deque_iterator<C2DVector>, _Deque_iterator<C2DVector>, _Deque_iterator<C2DVector>);
}