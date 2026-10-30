// std::vector<N3DAnimationBuilder::CSequenceInfo>::_M_fill_insert(CSequenceInfo* position, unsigned int n, const CSequenceInfo& x) -- STLport insert of n copies of a 52-byte animation-builder sequence record: slide the tail inside the spare capacity and fill the hole from a copy of x, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCSequenceInfo@N3DAnimationBuilder@@V?$allocator@VCSequenceInfo@N3DAnimationBuilder@@@std@@@std@@QAEXPAVCSequenceInfo@N3DAnimationBuilder@@IABV34@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB2E40, 296 bytes, thiscall `ret 0Ch`.
// Identity (proven, two sources): the landed vector<N3DAnimationBuilder::CSequenceInfo>::resize 0x00AB3290 calls this row
//   through the inline `insert(pos, n, x)` and binds exactly this `_M_fill_insert` spelling (reloc_bindings.tsv); the
//   landed __copy_backward_ptrs<CSequenceInfo*, CSequenceInfo*> 0x00AB13D0 names this row as its only caller; the
//   52-byte stride (`imul 4EC4EC4Fh ; sar edx,4`, the 13-dword `rep movsd` x_copy) is Ego_r.pdb CSequenceInfo
//   {long OriginalIndex; CMatrix3x4 GeneratedTransform;}. STLport 4.6 vector.c verbatim (the C3DQuad2 twin 0x00AE6E60).
// Label: the manifest's "Vector_ResizeWithAppearanceModifiers" is a descriptive guess (wrong).
// Callees: __uninitialized_copy<CSequenceInfo*, CSequenceInfo*> 0x00AB13A0 (twice), __copy_backward_ptrs<CSequenceInfo*,
//   CSequenceInfo*> 0x00AB13D0, fill<CSequenceInfo*, CSequenceInfo> 0x00AB10F0 (twice), uninitialized_fill_n<CSequenceInfo*,
//   unsigned int, CSequenceInfo> 0x00AB1410, _M_insert_overflow 0x00AB2CE0. Every free helper is __fastcall (first/last in
//   ecx/edx), as in the landed siblings.
// Shape: retail homes both `__false_type()` tags on the dead position argument slot ([esp+48h]), which still holds a live
//   value across the calls, so __uninitialized_copy and __copy_backward_ptrs get same-TU noinline bodies that never touch
//   the tag (the live-slot rule of 0x00AE82C0 / 0x00AE6E60).
extern "C" void __cdecl free(void* p);                             // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CMatrix3x4 {                              // Ego_r.pdb, 48 bytes
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};

namespace N3DAnimationBuilder {
class CSequenceInfo {                           // Ego_r.pdb, 52 bytes
public:
    long OriginalIndex;                         // +0x0
    CMatrix3x4 GeneratedTransform;              // +0x4
};
}

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00AB13A0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);         // 0x00AB1410
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                          _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _RandomAccessIter, class _BidirectionalIter>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward_ptrs(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                       _BidirectionalIter __result, const __false_type&)
{                                                                                              // 0x00AB13D0
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val);                     // 0x00AB10F0

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                       // 0x00AB2CE0
public:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}
}

template void std::vector<N3DAnimationBuilder::CSequenceInfo>::_M_fill_insert(N3DAnimationBuilder::CSequenceInfo*, unsigned int, const N3DAnimationBuilder::CSequenceInfo&);