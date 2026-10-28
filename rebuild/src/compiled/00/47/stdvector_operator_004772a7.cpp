// std::vector::operator=(const vector& __x) -- (vector<CEngineLocalDetailLayerDef>) assign definition records: reallocate, shrink or extend
// Leaf: ??4?$vector@VCEngineLocalDetailLayerDef@@V?$allocator@VCEngineLocalDetailLayerDef@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004772A7, 295 bytes, thiscall, `ret 4` (frame-pointer size module).
// Identity (proven): its only caller is CEngineLocalDetailGeneratorDef::Copy 0x00477288; the element operator= 0x00477A8A is CEngineLocalDetailLayerDef::operator= (family M043, proven), the copy constructor 0x004773F1 and __destroy_aux 0x00450A90 are bound to CEngineLocalDetailLayerDef.
// Callees: vector<CEngineLocalDetailLayerDef>::_M_allocate_and_copy 0x00477C6A, vector<CEngineLocalDetailLayerDef>::_M_clear 0x004773CE, CEngineLocalDetailLayerDef::operator= 0x00477A8A,
//   std::__destroy_aux<CEngineLocalDetailLayerDef*> 0x00450A90 (__fastcall), CEngineLocalDetailLayerDef copy constructor 0x004773F1.
// Shape (family M059, representative 0x0046F565): STLport 4.6 vector.c operator= with the __false_type helpers inline
//   (`__forceinline`): `optimize("y", off)` + `optimize("s", on)`. _M_clear is declared public as the existing binding of
//   0x0046F68C spells it.
// Header debt: the element is modelled by its retail size (0x54) with its out-of-line copy/assign and a virtual destructor.
#pragma optimize("y", off)
#pragma optimize("s", on)
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CEngineLocalDetailLayerDef {
public:
    CEngineLocalDetailLayerDef(const CEngineLocalDetailLayerDef& other);                 // 0x004773F1
    CEngineLocalDetailLayerDef& operator=(const CEngineLocalDetailLayerDef& other);      // 0x00477A8A
    virtual ~CEngineLocalDetailLayerDef();
    unsigned char _body[0x54 - 4];
};
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
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
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
__forceinline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
template <class _Tp> __forceinline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
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
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    unsigned int capacity() const { return unsigned int(this->_M_end_of_storage._M_data - this->_M_start); }
    vector& operator=(const vector& __x);
    protected:
    void _M_clear();
    public:
protected:
    template <class _ForwardIterator> _Tp* _M_allocate_and_copy(unsigned int __n, _ForwardIterator __first, _ForwardIterator __last);
};
template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        const unsigned int __xlen = __x.size();
        if (__xlen > capacity()) {
            _Tp* __tmp = _M_allocate_and_copy(__xlen, (const _Tp*)__x._M_start + 0, (const _Tp*)__x._M_finish + 0);
            _M_clear();
            this->_M_start = __tmp;
            this->_M_end_of_storage._M_data = this->_M_start + __xlen;
        }
        else if (size() >= __xlen) {
            _Tp* __i = __copy_ptrs((const _Tp*)__x._M_start + 0, (const _Tp*)__x._M_finish + 0, (_Tp*)this->_M_start, _TrivialAss());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const _Tp*)__x._M_start, (const _Tp*)__x._M_start + size(), (_Tp*)this->_M_start, _TrivialAss());
            __uninitialized_copy((const _Tp*)__x._M_start + size(), (const _Tp*)__x._M_finish + 0, this->_M_finish, _IsPODType());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}
template vector<CEngineLocalDetailLayerDef>& vector<CEngineLocalDetailLayerDef>::operator=(const vector<CEngineLocalDetailLayerDef>&);
}