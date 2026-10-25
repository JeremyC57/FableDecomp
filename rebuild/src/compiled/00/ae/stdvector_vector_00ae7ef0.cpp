// std::vector::~vector() -- (vector<C3DGroup2 >) out-of-line destructor: destroy every 0x10-byte element (its implicit destructor frees the inner VertexBlends array at +0x4 when it has a block), then free the vector's own block
// Leaf: ??1?$vector@VC3DGroup2@@V?$allocator@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE7EF0, 58 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: vector<C3DGroup2 >::~vector -- proven; evidence: the CParticleProgram destructor 0x00AE3670 (it stores the CParticleProgram vtable 0x0129F0D8) destroys NonSimGroups (+0x108) and SimGroups (+0xFC) with it: Ego_r.pdb CArray<C3DGroup2> members at Ego_r 0x12C / 0x11C less the 4-byte-smaller retail containers before them; the jmp thunk 0x00AE81D0 is the CArray-level destructor.
// Label: the manifest's "FreeStringVector" is descriptive / propagated across byte-identical bodies.
// Callees: free 0x00BFEA14 (twice: the element's inner block, then the vector's block).
// Body: STLport 4.6 `~vector() { _Destroy(_M_start, _M_finish); }` with the inline `~_Vector_base() { if (_M_start) deallocate(_M_start, ...); }`; the element destructors are implicit (the inner vector's
//   trivially destructible elements need no loop: STLport's __type_traits<T>::has_trivial_destructor picks the empty
//   __destroy_aux).
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    void _M_clear()                                             // protected in STLport 4.6; public as the landed bindings spell it
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
};
}
template <class T> class CArray : public std::vector<T> { };     // retail CArray: an STLport vector (12 bytes)
class C3DVertexBlend2 {                                       // 8 bytes (Ego_r.pdb)
public:
    unsigned long VertexIndex;
    float BlendValue;
};
class C3DGroup2 {                                             // 16 bytes (Ego_r.pdb 20)
public:
    long BoneIndex;                                           // +0x00
    CArray<C3DVertexBlend2> VertexBlends;                     // +0x04
};
namespace std {
template <> struct __type_traits<C3DVertexBlend2 > { typedef __true_type has_trivial_destructor; };
}
template class std::vector<C3DGroup2 >;