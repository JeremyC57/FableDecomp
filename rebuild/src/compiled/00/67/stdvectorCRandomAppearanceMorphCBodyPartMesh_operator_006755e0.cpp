// std::vector<CRandomAppearanceMorph::CBodyPartMesh>::operator=(const vector& x) -- STLport assignment: if x is not *this, grow into a fresh block (malloc, copy-construct, then _M_clear the old one) when x is larger than the capacity, else assign over the live elements and destroy the surplus or copy-construct the tail; size becomes x.size()
// Leaf: ??4?$vector@VCBodyPartMesh@CRandomAppearanceMorph@@V?$allocator@VCBodyPartMesh@CRandomAppearanceMorph@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006755e0, 311 bytes, thiscall, returns *this, `ret 4`.
// Identity (proven): Ego_r.pdb ??4?$vector@VCBodyPartMesh@CRandomAppearanceMorph@@...@QAEAAV01@ABV01@@Z; its __uninitialized_copy 0x00674FA0 is bound ??$__uninitialized_copy@PBVCBodyPartMesh@CRandomAppearanceMorph@@PAV12@@std@@..., the element is 24 bytes and its assignment (__copy 0x00674D80) copies MeshIndex, Priority, then the TextureMorphs CVectorMap (vector<pair<unsigned long, CTextureMorph> >::operator= 0x006741E0 + Dirty); callers are the landed __copy / fill over CArray<CBodyPartMesh> (0x00675790, 0x0086E920, 0x0086E950, 0x0086E770); bound only as the stand-in ??4CElement12_006755e0@@.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy<const T*, T*>(first, last, result, const __false_type&) 0x00674FA0,
//   vector::_M_clear() 0x00675530 (thiscall), __copy<const T*, T*, int>(first, last, result,
//   const random_access_iterator_tag&, int*) 0x00674D80 and _Destroy<T*>(first, last) 0x00675500 (__fastcall, out of line).
// Shape (family L032): STLport 4.6 vector.c operator= verbatim (element count by the 24-byte magic divide); the empty
//   tags are homed on the x argument slot, which VC7.1 does only when __copy and __uninitialized_copy have same-TU
//   noinline bodies (their real STLport bodies here; only operator= is graded).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
};
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)        // 0x00674D80
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)  // 0x00674FA0
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last);                  // 0x00675500
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
protected:
    void _M_clear();                                      // 0x00675530 (out of line)
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
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
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, (pointer)this->_M_start, __false_type());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start, __false_type());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish, __false_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}
}
class CRandomAppearanceMorph {
public:
    class CTextureMorph {                                  // Ego_r.pdb, 8 bytes
    public:
        unsigned long NewTexture;                         // +0x00
        unsigned long GroupID;                            // +0x04
    };
    class CBodyPartMesh;
};
template <class K, class V> class CKeyPairCompareLess { };  // empty comparator
template <class K, class V, class C>
class CVectorMap : public std::vector<std::pair<K, V> > {  // Ego_r.pdb: the vector base, then Compare, Dirty
public:
    C Compare;                                            // +0x0C
    bool Dirty;                                           // +0x0D
};
class CRandomAppearanceMorph::CBodyPartMesh {              // Ego_r.pdb, 24 bytes in retail
public:
    long MeshIndex;                                       // +0x00
    long Priority;                                        // +0x04
    CVectorMap<unsigned long, CTextureMorph, CKeyPairCompareLess<unsigned long, CTextureMorph> > TextureMorphs;  // +0x08
};
template std::vector<CRandomAppearanceMorph::CBodyPartMesh >& std::vector<CRandomAppearanceMorph::CBodyPartMesh >::operator=(const std::vector<CRandomAppearanceMorph::CBodyPartMesh >&);