// std::vector<pair<CCharString,unsigned long> >::_M_fill_insert(pair* position, unsigned int n, const pair& x) -- STLport insert of n copies of a (name, index) pair: slide the tail inside the spare capacity and fill the hole from a copy of x, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@U?$pair@VCCharString@@K@std@@V?$allocator@U?$pair@VCCharString@@K@std@@@2@@std@@QAEXPAU?$pair@VCCharString@@K@2@IABU32@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE7340, 278 bytes, thiscall `ret 0Ch`, no EH frame (STLport built without exceptions).
// Identity: the landed vector<pair<CCharString,unsigned long> >::resize 0x00AE7D10 (Ego_r.pdb ?resize@?$vector@U?$pair@
//   VCCharString@@K@std@@...@@QAEXI@Z; CParticleProgram::ExportParticles) calls it through the inline `insert(pos, n, x)`
//   and names it `vector::_M_fill_insert(pos, n, x)`; its callees are the landed pair<CCharString,unsigned long> helpers.
//   STLport 4.6 vector.c verbatim.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (x_copy.first), __uninitialized_copy<pair*, pair*>
//   0x009B2680 (twice), __copy_backward<pair*, pair*, int> 0x009B1E20 (behind the inline __copy_backward_ptrs),
//   uninitialized_fill_n<pair*, unsigned int, pair> 0x00AE5850, fill<pair*, pair> 0x00AE5820 (both fills tail-merged),
//   CCharString::~CCharString 0x0099EAE0 (x_copy), _M_insert_overflow 0x009B71B0. Free helpers are __fastcall.
// Shape: x_copy lives in the 8-byte frame (the string copy constructed in place, then second); retail homes both tags on
//   the live n slot (finish - n / n - elems_after are stored there and re-read), so __uninitialized_copy and __copy_backward
//   get same-TU noinline bodies that never touch the tag (the live-slot rule of 0x00AE82C0).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;                                    // +0x00
    CCharString(const CCharString& other);                           // 0x0099EC30
    ~CCharString();                                                  // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);          // 0x0099EFB0 (used by the __copy_backward body only)
};

extern "C" void __cdecl free(void* p);                             // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;                                                       // +0x00
    _T2 second;                                                      // +0x04
};

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x009B2680
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);         // 0x00AE5850
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                              // 0x009B1E20
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val);                     // 0x00AE5820

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                       // 0x009B71B0
public:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
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
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}
}

template void std::vector<std::pair<CCharString, unsigned long> >::_M_fill_insert(std::pair<CCharString, unsigned long>*,
                                                                                  unsigned int,
                                                                                  const std::pair<CCharString, unsigned long>&);