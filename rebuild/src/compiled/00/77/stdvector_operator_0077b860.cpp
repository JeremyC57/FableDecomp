// std::vector::operator=(const vector<CEngineTextureDecal>& x) -- STLport vector assignment of engine texture decals: reallocate and copy when x exceeds the capacity, else assign over the live elements and destroy the surplus or copy-construct the tail
// Leaf: ??4?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077B860, length 320 bytes (thiscall, `ret 4`, returns this).
// Identity: the catalog label "Vector_Assign_CBodyPartMesh" is propagated (Ego_r.pdb CBodyPartMesh is 28 bytes). The
//   20-byte element (`imul 66666667h ; sar edx,3`) is the retail CEngineTextureDecal {OriginalTextureIndex, DecalID,
//   vector<CCompositeBlendDesc> OverlaidBlends}: the old block goes through vector<CEngineTextureDecal>::_M_clear
//   0x0070B1C0 (the landed _M_insert_overflow 0x0070B240's callee) and the element copy/assignment helpers copy two
//   words plus a vector of 16-byte CCompositeBlendDescs. Caller: NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh
//   0x0077B350 (the STLport COMDATs of that TU).
// Callees: malloc 0x00BFEA0E (allocator::allocate), __uninitialized_copy<const T*, T*> 0x0077B700 (twice), _M_clear
//   0x0070B1C0, __copy(first, last, result, random_access_iterator_tag, int*) 0x0077B7A0 (twice), _Destroy(first, last)
//   0x00709940.
// Shape: byte twin of 0x0076B1E0 (vector<ParticleAndEndPoints>::operator=): STLport 4.x vector.c `operator=` verbatim
//   with _M_allocate_and_copy and __copy_ptrs inline; the const-pointer helpers are same-TU `__declspec(noinline)`
//   bodies that never read their tags, so VC7.1 homes every tag on the dead `x` slot (which also keeps x._M_finish).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

class CCompositeBlendDesc;

class CCompositeBlendDescVector {                           // vector<CCompositeBlendDesc> (retail STLport, 12 bytes)
public:
    CCompositeBlendDesc* _M_start;
    CCompositeBlendDesc* _M_finish;
    CCompositeBlendDesc* _M_end_of_storage;
    CCompositeBlendDescVector(const CCompositeBlendDescVector& __x);                // inline in the helpers
    CCompositeBlendDescVector& operator=(const CCompositeBlendDescVector& __x);     // 0x006CADE0
    ~CCompositeBlendDescVector();                                                   // inline in the helpers
};

class CEngineTextureDecal { public:                                // Ego_r.pdb; 20 bytes retail
    long                      OriginalTextureIndex;         // +0x00
    long                      DecalID;                      // +0x04
    CCompositeBlendDescVector OverlaidBlends;               // +0x08
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
__declspec(noinline) void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{                                                                                              // 0x00709940
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x0077B700
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{                                                                                              // 0x0077B7A0
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    void _M_clear();                                        // 0x0070B1C0
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, _IsPODType());
        return __result;
    }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
};

template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        const size_type __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
            _M_clear();
            this->_M_start = __tmp;
            this->_M_end_of_storage._M_data = this->_M_start + __xlen;
        }
        else if (size() >= __xlen) {
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0,
                                      (pointer)this->_M_start, _TrivialAss());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start,
                        _TrivialAss());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0,
                                 this->_M_finish, _IsPODType());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template vector<CEngineTextureDecal>& vector<CEngineTextureDecal>::operator=(const vector<CEngineTextureDecal>&);

}