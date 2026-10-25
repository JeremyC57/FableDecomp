// std::vector::operator=(const vector<ParticleAndEndPoints>& x) -- STLport vector assignment of an augmentation particle-description list: reallocate and copy when x exceeds the capacity, else assign over the live elements and destroy the surplus or copy-construct the tail
// Leaf: ??4?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076B1E0, length 320 bytes (thiscall, `ret 4`, returns this).
// Identity: the catalog label "Vector_Assign_ReserveCapacity" is descriptive. The 20-byte element is Ego_r.pdb
//   ParticleAndEndPoints (`imul 66666667h ; sar edx,3`); every callee is a landed helper of that vector (the inner vector
//   of CTCObjectAugmentations::ParticleEffects): _M_clear 0x0076B1A0, `_Destroy(first, last)` 0x0076AE90 (its only
//   caller is this body), __uninitialized_copy<const T*, T*> 0x00769930. Callers: CTCObjectAugmentations::
//   ClearAllParticleEffects 0x007674A0 and the copy 0x0076BA70.
// Callees: malloc 0x00BFEA0E (allocator::allocate), __uninitialized_copy 0x00769930 (twice), _M_clear 0x0076B1A0,
//   __copy(first, last, result, random_access_iterator_tag, int*) 0x00769B90 (twice), _Destroy 0x0076AE90.
// Shape: STLport 4.x vector.c `operator=` verbatim: `if (&x != this) { xlen = x.size(); if (xlen > capacity()) { tmp =
//   _M_allocate_and_copy(xlen, x._M_start, x._M_finish); _M_clear(); _M_start = tmp; _M_end_of_storage = _M_start + xlen; }
//   else if (size() >= xlen) { i = __copy_ptrs(x._M_start, x._M_finish, _M_start, __false_type()); _Destroy(i, _M_finish); }
//   else { __copy_ptrs(x._M_start, x._M_start + size(), _M_start, ..); __uninitialized_copy(x._M_start + size(),
//   x._M_finish, _M_finish, ..); } _M_finish = _M_start + xlen; } return *this;` -- _M_allocate_and_copy and __copy_ptrs
//   inline; the const-pointer helpers are same-TU `__declspec(noinline)` bodies that never read their tags, so VC7.1
//   homes every tag on the dead `x` slot (which also keeps x._M_finish).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    ~CCharString();                                         // 0x0099EAE0
};

struct ParticleAndEndPoints {                               // Ego_r.pdb, 20 bytes
    CCharString ParticleName;                               // +0x00
    CCharString HelperName;                                 // +0x04
    long        StartPoint;                                 // +0x08
    long        EndPoint;                                   // +0x0C
    long        PeriodicFrames;                             // +0x10
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
{                                                                                              // 0x0076AE90
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00769930
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{                                                                                              // 0x00769B90
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
    void _M_clear();                                        // 0x0076B1A0
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

template vector<ParticleAndEndPoints>& vector<ParticleAndEndPoints>::operator=(const vector<ParticleAndEndPoints>&);

}