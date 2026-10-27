// std::_Destroy(C3DGroup2* __first, C3DGroup2* __last) -- destroy a range of C3DGroup2s: each element's vector frees its block
// Leaf: ??$_Destroy@PAVC3DGroup2@@@std@@YIXPAVC3DGroup2@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE7EC0, 44 bytes, `__fastcall` (first ecx, last edx), plain `ret`.
// Identity (probable): its only caller 0x00AE7FE0 is the STLport operator= of a 16-byte-element vector (`sar 4`) that the vector<C3DGroup2> reserve 0x00AE80F0 also reaches; Ego_r.pdb C3DGroup2 is {long BoneIndex; CArray<C3DVertexBlend2> VertexBlends;} -- 16 bytes with the retail 12-byte vector, whose block at +4 is what each pass frees.
// Callees: free 0x00BFEA14 (allocator::deallocate inside the member vector's destructor).
// Shape (family M188, representative 0x00675500): STLport 4.6 `_Destroy(first, last)` -> `__destroy` -> `__destroy_aux(..., __false_type())`,
//   `for (; __first != __last; ++__first) _Destroy(&*__first);` all inline (the landed 0x00CDB820 spelling) --
//   the element's implicit destructor is the member vector's (its own elements trivially destructible), and the STLport
//   _Vector_base null test `if (_M_start != 0) deallocate(...)` is what makes VC test first == last before saving esi and walk a
//   pointer to the member (+0x4), testing `esi - 4` against last (without it the loop indexes off first). Speed TU.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
};
struct __false_type { __false_type() { } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator, class _Tp>
inline void __fastcall __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _ForwardIterator>
inline void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, __first);
}
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
class C3DVertexBlend2 {                                    // Ego_r.pdb, 8 bytes
public:
    unsigned long VertexIndex;                            // +0x00
    float BlendValue;                                     // +0x04
};
template <class T> class CArray : public std::vector<T> { };   // Ego_r.pdb: a vector with no members of its own
class C3DGroup2 {                                          // Ego_r.pdb, 16 bytes in retail
public:
    long BoneIndex;                                       // +0x00
    CArray<C3DVertexBlend2> VertexBlends;                 // +0x04
};
namespace std {
template void __fastcall _Destroy<C3DGroup2*>(C3DGroup2* __first, C3DGroup2* __last);
}