// std::vector<CFileInstaller::CBufferChunk>::_M_fill_insert(CFileInstaller::CBufferChunk* position, unsigned int n, const CFileInstaller::CBufferChunk& x) -- STLport insert of n copies of an 8-byte buffer chunk: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCBufferChunk@CFileInstaller@@V?$allocator@VCBufferChunk@CFileInstaller@@@std@@@std@@QAEXPAVCBufferChunk@CFileInstaller@@IABV34@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C75A0, 274 bytes, thiscall `ret 0Ch`.
// Identity: called by the landed vector<CFileInstaller::CBufferChunk>::resize 0x009C7850 (the CFileInstaller::BufferChunks
//   array behind CFileInstaller::InitBuffers); the frame local's destructor is the landed ~CBufferChunk 0x009C6020.
//   Catalog label "std::vector_resize_8byte" is descriptive.
// Callees (the /Gr TU's `__fastcall` STLport helpers): __uninitialized_copy(first, last, result, const __false_type&)
//   0x009C6510 (twice, `ret 8`), __copy_backward_ptrs(first, last, result, const __false_type&) 0x009C6540 (`ret 8`),
//   uninitialized_fill_n(first, n, x) 0x009C6570 (`ret 4`: the tag helper inlined into the wrapper), fill(first, last,
//   val) 0x009C5EC0 (`ret 4`), _M_insert_overflow 0x009C73E0 (thiscall, atend defaulted false: `push 0`),
//   ~CBufferChunk 0x009C6020.
// Shape: the 0x0098D360 STLport `_M_fill_insert` model; `__x_copy` (two dword moves, the implicit copy) is the only
//   frame local and is destroyed after the shared fill tail; `__elems_after` is homed in the dead `__x` slot.
inline void* operator new(unsigned int, void* p) { return p; }

class CFileInstaller {
public:
    class CBufferChunk {                                // Ego_r.pdb, 8 bytes
    public:
        unsigned char* Mem;                             // +0x00
        bool           Used;                            // +0x04
        bool           IsInput;                         // +0x05
        ~CBufferChunk();                                // 0x009C6020
    };
};

namespace std {

struct __false_type { __false_type() { } };

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
{                                                                               // 0x009C6510
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
{                                                                               // 0x009C6570
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
__declspec(noinline) _BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first,
                                                                         _BidirectionalIter1 __last,
                                                                         _BidirectionalIter2 __result,
                                                                         const __false_type&)
{                                                                               // 0x009C6540
    for (int __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                               // 0x009C5EC0
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
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                              // 0x009C73E0
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

template void vector<CFileInstaller::CBufferChunk>::_M_fill_insert(CFileInstaller::CBufferChunk*, unsigned int,
                                                                   const CFileInstaller::CBufferChunk&);

}