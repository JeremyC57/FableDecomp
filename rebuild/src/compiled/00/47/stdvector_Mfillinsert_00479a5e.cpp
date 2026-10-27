// std::vector::_M_fill_insert(CEngineLocalDetailLayerDef* __position, unsigned int __n, const CEngineLocalDetailLayerDef& __x) -- (vector<CEngineLocalDetailLayerDef>) insert n copies of x
// Leaf: ?_M_fill_insert@?$vector@VCEngineLocalDetailLayerDef@@V?$allocator@VCEngineLocalDetailLayerDef@@@std@@@std@@QAEXPAVCEngineLocalDetailLayerDef@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00479A5E, 340 bytes, thiscall, `ret 0Ch` (frame-pointer size module).
// Identity (proven): its only caller is the landed vector<CEngineLocalDetailLayerDef>::resize 0x004799C6; copy constructor 0x004773F1 is bound to CEngineLocalDetailLayerDef and operator= 0x00477A8A is its operator= (family M043).
// Callees: CEngineLocalDetailLayerDef copy constructor 0x004773F1, CEngineLocalDetailLayerDef::operator= 0x00477A8A, uninitialized_fill_n<CEngineLocalDetailLayerDef*, unsigned int, CEngineLocalDetailLayerDef> 0x00479C94
//   (__fastcall), ~CEngineLocalDetailLayerDef 0x0047747C (the x copy), vector<CEngineLocalDetailLayerDef>::_M_insert_overflow 0x00479BB2.
// Shape (family M065, representative 0x004766C2): the M051 model (STLport vector.c `_M_fill_insert`, helpers `__forceinline`,
//   `optimize("y", off)` + `optimize("s", on)`) over a record with out-of-line copy and assignment; the x copy's destructor is the record's own out-of-line destructor.
// Header debt: the record is modelled by its retail size (0x54) with its copy constructor, assignment and destructor.
#pragma optimize("y", off)
#pragma optimize("s", on)
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CEngineLocalDetailLayerDef {
public:
    CEngineLocalDetailLayerDef(const CEngineLocalDetailLayerDef& other);                 // 0x004773F1
    CEngineLocalDetailLayerDef& operator=(const CEngineLocalDetailLayerDef& other);      // 0x00477A8A
    virtual ~CEngineLocalDetailLayerDef();                       // 0x0047747C
    unsigned char _body[0x54 - 4];
};
namespace std {
struct __false_type { };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
__forceinline _BidirectionalIter2 __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BI1, class _BI2>
__forceinline _BI2 __copy_backward_ptrs(_BI1 __first, _BI1 __last, _BI2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, (int*)0);
}
template <class _ForwardIter, class _Tp>
__forceinline void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    void _M_fill_insert(_Tp* __position, unsigned int __n, const _Tp& __x);
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(_Tp* __position, unsigned int __n, const _Tp& __x)
{
    if (__n != 0) {
        if (unsigned int(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const unsigned int __elems_after = this->_M_finish - __position;
            _Tp* __old_finish = this->_M_finish;
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
template void vector<CEngineLocalDetailLayerDef>::_M_fill_insert(CEngineLocalDetailLayerDef*, unsigned int, const CEngineLocalDetailLayerDef&);
}