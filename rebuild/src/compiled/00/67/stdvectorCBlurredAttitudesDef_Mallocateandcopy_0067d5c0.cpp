// std::vector<CBlurredAttitudesDef>::_M_allocate_and_copy(size_type n, const_pointer first, const_pointer last) -- STLport: allocate room for n 0x48-byte defs and copy-construct [first, last) into it
// Leaf: ??$_M_allocate_and_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067D5C0, 78 bytes, thiscall with `this` unused (the allocator is
//   empty), `ret 0Ch`, returns the new block.
// Identity: the manifest's "std::vector<CConversation>::allocate_and_construct" is propagated (CConversation is not
//   0x48 bytes in retail). The element copy constructor 0x0067C6D0 is the landed CBlurredAttitudesDef copy constructor
//   (0x48 bytes: CParentDefClassBase 0x3C + the Attitudes vector), and the only caller 0x0067D49E is
//   vector<CBlurredAttitudesDef>::operator= 0x0067D450 (COpinionReactionManagerDef::operator=, landed 0x0067CE50).
// Callees: malloc 0x00BFEA0E (allocate(n) = `n ? malloc(n * 0x48) : 0`), CBlurredAttitudesDef copy ctor 0x0067C6D0.
// Shape: STLport 4.6 `pointer result = _M_end_of_storage.allocate(n); __uninitialized_copy(first, last, result,
//   _IsPODType()); return result;` (no EH in this TU) with the __false_type copy loop inline: placement new behind
//   its null test, VC7.1 strength-reducing the destination to `first + (result - first)` (edi).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CBlurredAttitudesDef {                                // 0x48 bytes in retail (only the copy matters here)
public:
    virtual ~CBlurredAttitudesDef();
    unsigned char Body[0x44];
    CBlurredAttitudesDef(const CBlurredAttitudesDef& other);   // 0x0067C6D0
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Param_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Param_Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    protected:
    template <class _ForwardIterator>
    _Tp* _M_allocate_and_copy(unsigned int __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        _Tp* __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    public:
};

template CBlurredAttitudesDef* vector<CBlurredAttitudesDef>::_M_allocate_and_copy<const CBlurredAttitudesDef*>(
    unsigned int, const CBlurredAttitudesDef*, const CBlurredAttitudesDef*);

} // namespace std