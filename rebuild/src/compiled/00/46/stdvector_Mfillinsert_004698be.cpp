// std::vector::_M_fill_insert(iterator __position, size_type __n, const value_type& __x) -- (vector<ECreatureGeneratorGenerateType >) STLport insert of n copies of x for a 4-byte element: shift the tail inside the capacity, else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@W4ECreatureGeneratorGenerateType@@V?$allocator@W4ECreatureGeneratorGenerateType@@@std@@@std@@QAEXPAW4ECreatureGeneratorGenerateType@@IAB
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004698BE, 267 bytes (size-optimised TU, frame pointer, odd entry), thiscall, `ret 0Ch`.
// Identity: std::vector<ECreatureGeneratorGenerateType >::_M_fill_insert -- proven; evidence: the call chain CCreatureGenerationFamilyDef::Transfer 0x004696C1 (GenerationTypes +0x50) -> Transfer<vector<ECreatureGeneratorGenerateType> > 0x004696FA -> GFSerialiseVectorBinaryIn 0x0046975A -> resize 0x00469851; the element type
//   is the same as G062's 0x0045C64D (its only caller is the landed CCreatureGenerationFamilyDef::operator= 0x0045C614, assigning GenerationTypes at +0x50 (Ego_r.pdb +0x58 after Creatures, DifficultyLevel, GeneratorObject; retail vectors are 12 bytes)). STLport spelling. Family G064 (seven _M_fill_insert of 4-byte elements;
//   catalogue "Vector_Insert" is behavioural).
// Callees: uninitialized_fill_n<T*, unsigned int, T>(first, n, x) 0x004699C9 (out of line, __fastcall, `ret 4`),
//   _M_insert_overflow(pos, x, const __false_type&, fill_len, atend) 0x004699E4 (G065).
// Shape: STLport 4.6 vector.c verbatim; __x_copy and __elems_after homed in the dead __x / __n slots; the overflow tag is
//   value-initialised (`stosb`), so __false_type has no user constructor in this TU; the copy / copy_backward / fill
//   helpers are __forceinline under /Os.
#pragma optimize("y", off)
#pragma optimize("s", on)
inline void* operator new(unsigned int, void* p) { return p; }
enum ECreatureGeneratorGenerateType { ECreatureGeneratorGenerateType_Force32 = 0x7fffffff };
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
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);
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
                            bool __atend = false);         // 0x004699E4
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
typedef ECreatureGeneratorGenerateType _Elem;
template void vector<_Elem>::_M_fill_insert(_Elem*, unsigned int, const _Elem&);
}