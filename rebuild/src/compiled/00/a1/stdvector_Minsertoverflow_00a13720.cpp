// std::vector::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- (vector<CPlane >) STLport growth insert for a 16-byte element with a user (memberwise) copy constructor: allocate size + max(size, fill_len) elements, copy the head out of line, construct x once (or fill out of line fill_len times), copy the tail unless atend, free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@VCPlane@@V?$allocator@VCPlane@@@std@@@std@@IAEXPAVCPlane@@ABV3@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A13720, 209 bytes (+ int3 padding to 224), thiscall, `ret 14h` (five arguments).
// Identity (proven): std::vector<CPlane >::_M_insert_overflow; evidence: its callers are vector<CPlane>::insert(pos, n, x) 0x00AAB570 (0x00AAB6A0; the landed C3DMeshFileConvexVolumeChunk::ReadFromFile 0x00AAAB10 binds it for its CArray<CPlane> Planes), the chunk's push_back stub 0x00AAAAC0 (0x00AAAB03: `add ecx,30h`, Planes at retail +0x30 = Ego_r.pdb +0x38 less the 8-byte-smaller C3DMeshFileChunk base) and the push_back stub 0x00A138E0.
// Label: the manifest's "Vector_InsertFill_Copy" is propagated / behavioural (family H043: six byte-identical bodies, each its own
//   COMDAT: retail has no identical-code folding, so each address is one element type).
// Callees: malloc 0x00BFEA0E (allocator::allocate), this element's own out-of-line STLport
//   `__uninitialized_copy(first, last, result, const __false_type&)` 0x00A11210 (__fastcall, `ret 8`, head and tail) and
//   `__uninitialized_fill_n(first, n, x, const __false_type&)` 0x00A11250 (__fastcall), free 0x00BFEA14 (_M_clear).
// Model: the landed G055 vector::_M_insert_overflow 0x00B8F840 (rebuild/src/compiled/00/b8) with a trivially destroyed
//   16-byte element (no destroy loop in _M_clear) and both helpers' bodies in the TU (noinline, __fastcall): the
//   fill_len == 1 arm constructs x through the PDB's user copy constructor, whose memberwise copy alternates two
//   registers as retail does (the implicit copy uses one; 207v209 without it).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
class C3DVector {                                          // Ego_r.pdb, 12 bytes
public:
    float X;
    float Y;
    float Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // PDB ??0C3DVector@@QAE@ABV0@@Z
};
class CPlane {                                             // Ego_r.pdb, 16 bytes
public:
    C3DVector Normal;                                      // +0x0
    float     Distance;                                    // +0xC
    CPlane(const CPlane& other)                            // PDB ??0CPlane@@QAE@ABV0@@Z
        : Normal(other.Normal), Distance(other.Distance) { }
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                          // same-TU body (retail's TU holds it): lets the tag share the live atend slot
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
#pragma auto_inline(on)
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{                                                          // same-TU body as well
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
typedef CPlane _Elem;
template void vector<_Elem>::_M_insert_overflow(_Elem*, const _Elem&, const __false_type&, unsigned int, bool);
} // namespace std