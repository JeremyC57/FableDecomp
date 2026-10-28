// std::vector::operator=(const vector& x) -- (vector<CBlurredAttitudesDef>) STLport assignment of 0x48-byte blurred-attitude defs: reallocate-and-copy when x outgrows the capacity, else assign over the live prefix and destroy the surplus, or assign the prefix and copy-construct the tail; finish = start + x.size(); returns *this
// Leaf: ??4?$vector@VCBlurredAttitudesDef@@V?$allocator@VCBlurredAttitudesDef@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067D450, 368 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): the 72-byte element (`imul 38E38E39h ; sar edx,4`) is copy-constructed through 0x0067C6D0, the landed
//   CBlurredAttitudesDef copy constructor (vtables 0x01230CBC CParentDefClassBase -> 0x0125B8AC CBlurredAttitudesDef);
//   the allocate-and-copy 0x0067D5C0 is landed as vector<CBlurredAttitudesDef>::_M_allocate_and_copy and names this body
//   (0x0067D49E) as its only caller; the only caller of this body is COpinionReactionManagerDef::operator= 0x0067CE50
//   (call at 0x0067CE81). The catalog's "vector<CConversation>::operator_assign" is propagated.
// Callees: _M_allocate_and_copy 0x0067D5C0 (thiscall), _M_clear 0x0067B7E0 (each element's virtual destructor, then the
//   null-tested free), CBlurredAttitudesDef::operator= 0x0067C490 (the inline __copy loops), _Destroy(first, last)
//   0x0067B7B0 (`__fastcall`, the surplus), CBlurredAttitudesDef::CBlurredAttitudesDef(const CBlurredAttitudesDef&)
//   0x0067C6D0 (the inline tail __uninitialized_copy).
// Model: STLport 4.6 vector::operator= verbatim (as the landed 0x00888EE0 / 0x00F27BC0 rows) with the random-access __copy
//   and the tail copy inline (`__forceinline`: VC7.1 will not inline a plain inline template holding a loop),
//   _M_allocate_and_copy / _M_clear / the range _Destroy out of line (declarations only).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CBlurredAttitudesDef {                                // 0x48 bytes in retail (CParentDefClassBase 0x3C + Attitudes)
public:
    virtual ~CBlurredAttitudesDef();
    unsigned char Body[0x44];
    CBlurredAttitudesDef(const CBlurredAttitudesDef& other);               // 0x0067C6D0
    CBlurredAttitudesDef& operator=(const CBlurredAttitudesDef& other);    // 0x0067C490
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

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
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
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
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last);               // 0x0067B7B0

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;

    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last);   // 0x0067D5C0
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
protected:
    void _M_clear();                                                // 0x0067B7E0
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
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish,
                                 _IsPODType());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template vector<CBlurredAttitudesDef>& vector<CBlurredAttitudesDef>::operator=(const vector<CBlurredAttitudesDef>&);

} // namespace std