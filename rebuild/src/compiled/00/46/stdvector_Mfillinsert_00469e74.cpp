// std::vector::_M_fill_insert(iterator __position, size_type __n, const value_type& __x) -- (vector<CTextureReplacementDef::CEntry >) STLport insert of n copies of x for an 8-byte element: shift the tail inside the capacity, else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCEntry@CTextureReplacementDef@@V?$allocator@VCEntry@CTextureReplacementDef@@@std@@@std@@QAEXPAVCEntry@CTextureReplacementDef@@IABV34@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00469E74, 305 bytes (size-optimised TU, frame pointer, odd entry), thiscall, `ret 0Ch`.
// Identity (proven): std::vector<CTextureReplacementDef::CEntry >::_M_fill_insert; evidence: its only caller is vector<CTextureReplacementDef::CEntry>::resize 0x00469E00 (site 0x00469E34), called by GFSerialiseVectorBinaryIn 0x00469CB2 from CPersistContext::Transfer 0x00469C3E of CTextureReplacementDef::Transfer 0x00469C2E (slot 18 of vtable 0x01233074, RTTI .?AVCTextureReplacementDef@@) on this+0x28 (Ego_r.pdb CTextureReplacementDef::Entries, CArray<CTextureReplacementDef::CEntry>)
// Label: the manifest's "Vector_Insert" is a behavioural label; this row is vector<CTextureReplacementDef::CEntry >::_M_fill_insert (family J064: five 8-byte-element instances, each its own type).
// Callees: uninitialized_fill_n<T*, unsigned int, T>(first, n, x) 0x00469FA5 (out of line, __fastcall, `ret 4`),
//   _M_insert_overflow(pos, x, const __false_type&, fill_len, atend) 0x00469FC8.
// Shape: STLport 4.6 vector.c verbatim (agent951's 4-byte model 0x0046CBF1, unchanged but for the element): __x_copy lives in the two-dword frame,
//   each 8-byte element moves as two dwords; the overflow tag is value-initialised (`stosb`); copy / copy_backward / fill are __forceinline under /Os.
// Element: CTextureReplacementDef::CEntry, 8 bytes by Ego_r.pdb CPersistTraits<CTextureReplacementDef::CEntry>; its members are the Ego_r.pdb nested CEntry {long OriginalTexture; long NewTexture} (the PDB keys nested classes by the bare name CEntry; this is the only 8-byte CEntry about textures -- probable member names, irrelevant to the bytes)
#pragma optimize("y", off)
#pragma optimize("s", on)
inline void* operator new(unsigned int, void* p) { return p; }
class CTextureReplacementDef {
public:
    class CEntry {                                         // 8 bytes (Ego_r.pdb CPersistTraits<CTextureReplacementDef::CEntry> 8)
    public:
        long OriginalTexture;
        long NewTexture;
    };
};
namespace std {
struct __false_type { };
struct random_access_iterator_tag { };
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);   // 0x00469FA5
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__forceinline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                                 const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
__forceinline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                       _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp>
__forceinline void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
template <class _Tp> class allocator { };
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;                      // __type_traits of a class/enum element
    typedef __false_type _IsPODType;
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);         // 0x00469FC8
public:
    void _M_fill_insert(iterator __position, size_type __n, const _Tp& __x);
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
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}
typedef CTextureReplacementDef::CEntry _Elem;
template void vector<_Elem>::_M_fill_insert(_Elem*, unsigned int, const _Elem&);
}