// std::vector::operator=(const vector<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph> >& x) -- STLport copy assignment of the per-body-part texture morph maps: reallocate and copy when x outgrows the capacity, else assign over the live prefix and destroy or copy-construct the rest
// Leaf: ??4?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006753F0, 272 bytes (255 without padding), thiscall `ret 4`, returns `*this`.
//   Callers: CRandomAppearanceMorph::operator= 0x00675B60 (landed; BodyPartTextureMorphs +0x0C) and 0x00675578.
// Callees (all out of line in this TU): malloc 0x00BFEA0E (allocate), __uninitialized_copy(.., const __false_type&)
//   0x00674CE0 (`__fastcall`), _M_clear 0x00675340 (thiscall: the element destroy loop + free), __copy(.., const
//   random_access_iterator_tag&, int*) 0x00674A40 (`__fastcall`, behind __copy_ptrs(.., __false_type)), _Destroy(first,
//   last) 0x00675310 (`__fastcall`).
// Label correction: the catalog "Vector_Assign_16byte_CFeatDef" -- this is the CArray<CVectorMap<unsigned long,
//   CTextureMorph> > assignment (16-byte retail CVectorMap: vector, empty comparator, sorted flag at +0x0D).
// Shape: STLport 4.6 operator= verbatim: `_M_allocate_and_copy` inlined around the out-of-line __uninitialized_copy,
//   `_M_clear()` stays a call, `__copy_ptrs(.., __false_type)` = `__copy(.., random_access_iterator_tag(), (int*)0)`; the
//   helpers are same-TU noinline bodies (auto_inline off) that never touch their tags, so VC7.1 homes x._M_finish and
//   every empty tag in x's dead argument slot and hoists the shared `push 0` above the size test, as retail.
extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                               // 0x00BFEA14
inline void* operator new(unsigned int, void* __p) throw() { return __p; }

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}

class CRandomAppearanceMorph {
public:
    class CTextureMorph {
    public:
        unsigned long NewTexture;                                                    // +0x00
        unsigned long GroupID;                                                       // +0x04
    };
};

template <class K, class V> class CKeyPairCompareLess { };

template <class T> class CArray {                                                    // retail CArray<T>: an STLport vector
public:
    T* m_first;
    T* m_last;
    T* m_end;
    CArray(const CArray& other);
    CArray& operator=(const CArray& other);
    ~CArray() { if (m_first) free(m_first); }
};

template <class K, class V, class C> class CVectorMap {                              // retail: 16 bytes
public:
    CArray<std::pair<K, V> > Vector;                                                 // +0x00
    C                        Compare;                                                // +0x0C (empty)
    bool                     Sorted;                                                 // +0x0D
};

typedef CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph,
                   CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > CTextureMorphMap;

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                                                     // 0x00674CE0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{                                                                                     // 0x00674A40
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{                                                                                     // 0x00675310
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

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
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, _IsPODType());
        return __result;
    }
    __declspec(noinline) void _M_clear()                                             // 0x00675340
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    vector& operator=(const vector& __x);
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
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, (pointer)this->_M_start, _TrivialAss());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start, _TrivialAss());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish, _IsPODType());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template vector<CTextureMorphMap>& vector<CTextureMorphMap>::operator=(const vector<CTextureMorphMap>&);
}