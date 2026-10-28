// std::vector<NGameText::CTextTag>::_M_fill_insert(NGameText::CTextTag* position, unsigned int n, const NGameText::CTextTag& x) -- STLport insert of n copies of an 8-byte text tag {Pos, Name}: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCTextTag@NGameText@@V?$allocator@VCTextTag@NGameText@@@std@@@std@@QAEXPAVCTextTag@NGameText@@IABV34@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CAF60, 280 bytes, thiscall `ret 0Ch`.
// Identity: called by the landed vector<NGameText::CTextTag>::resize 0x009CB7D0 (NGameText::CTextEntry::Tags, behind
//   CTextEntry::ReadFromDataStream); the element is Ego_r.pdb CTextTag { long Pos; CCharString Name; }.
//   Catalog label "std::vector_insert_pair_float_string" is descriptive (the first word is a long, not a float).
// Callees: CCharString copy ctor 0x0099EC30 and dtor 0x0099EAE0 (the implicit CTextTag copy/dtor of `__x_copy`);
//   the /Gr TU's `__fastcall` STLport helpers __uninitialized_copy(.., const __false_type&) 0x009CA190 (twice, `ret 8`),
//   __copy_backward(.., const random_access_iterator_tag&, int*) 0x009C9DD0 (`ret 0Ch`: __copy_backward_ptrs inlined,
//   `push 0` is the distance-type null), uninitialized_fill_n(first, n, x) 0x009CA220 (`ret 4`), fill 0x009CA1F0
//   (`ret 4`), _M_insert_overflow 0x009CAD20 (thiscall, `push 0` atend).
// Shape: the 0x0098D360 / 0x009C75A0 STLport `_M_fill_insert` model.
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* m_pData;                                      // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace NGameText {
class CTextTag {                                        // Ego_r.pdb, 8 bytes
public:
    long        Pos;                                    // +0x00
    CCharString Name;                                   // +0x04
};
}

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
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
{                                                                               // 0x009CA190
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{                                                                               // 0x009CA220
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                               // 0x009C9DD0
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
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                               // 0x009CA1F0
    for (; __first != __last; ++__first)
        *__first = __val;
}

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
    typedef _Tp* iterator;
    typedef __false_type _IsPODType;
    typedef __false_type _TrivialAss;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                              // 0x009CAD20
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
            } else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        } else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}

template void vector<NGameText::CTextTag>::_M_fill_insert(NGameText::CTextTag*, unsigned int,
                                                                   const NGameText::CTextTag&);

}