// std::vector<CIVCountedPointer<CManagedTextureData> >::_M_fill_insert(CIVCountedPointer<CManagedTextureData>* position, unsigned int n, const CIVCountedPointer<CManagedTextureData>& x) -- STLport insert of n copies of an intrusive counted texture pointer: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Grade: DIFFER(285v276) -- only the inline ~CIVCountedPointer of `__x_copy` at the tail: ours `mov eax,[x] ; test ; je ; dec [eax+4] ; mov ecx,[x] ; mov eax,[ecx+4] ; test eax,eax ; jne` (x_copy reloaded after the dec, aliasing), retail `mov ecx,[x] ; test ; je ; dec [ecx+4] ; jne` (flags from the dec)
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6BCD0, 276 bytes, thiscall `ret 0Ch`.
// Identity: reloc_bindings spells it `?_M_fill_insert@?$vector@V?$CIVCountedPointer@VCManagedTextureData@@@@V?$allocator@..`
//   from the landed (proven) resize 0x00A6C020 (CPreallocTexturePool::Textures, Ego_r.pdb CArray<CIVCountedPointer<
//   CManagedTextureData> >); 4-byte element (`sar 2`). Catalog label "Vector_InsertN_DetailPlacementGrid" is propagated.
// Callees (the /Gr TU's `__fastcall` STLport helpers): __uninitialized_copy(.., const __false_type&) 0x00A6AFD0 (twice),
//   __copy_backward(.., const random_access_iterator_tag&, int*) 0x00A6B0E0, uninitialized_fill_n 0x00A6B340 (`ret 4`),
//   fill 0x00A6B000 (`ret 4`, shared tail), _M_insert_overflow 0x00A6BA60 (thiscall, `push 0` atend); the pointee's
//   slot-1 virtual (the release when the last reference goes) from the inline ~CIVCountedPointer of `__x_copy`.
// Shape: the 0x009CAF60 STLport `_M_fill_insert` model; the 4-byte `__x_copy` (inline copy ctor: `inc [p+4]`) is homed in
//   the dead `__x` argument slot, so there is no frame.
inline void* operator new(unsigned int, void* p) { return p; }

class CIVCountedPointeeBase { public:                       // the landed 0x0083CE60 model
    virtual void Destroy(int flags);                        // slot 0
    virtual void Delete();                                  // slot 1
    unsigned long IVRefCount;                               // +0x04
};

template <class T> class CIVCountedPointer {                // 4 bytes (Ego_r.pdb)
public:
    CIVCountedPointeeBase* Object;                          // +0x00
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    ~CIVCountedPointer()
    {
        if (Object && --Object->IVRefCount == 0)
            Object->Delete();
    }
    CIVCountedPointer& operator=(const CIVCountedPointer& other);
};
class CManagedTextureData;

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
{                                                                               // 0x00A6AFD0
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
{                                                                               // 0x00A6B340
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                               // 0x00A6B0E0
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
{                                                                               // 0x00A6B000
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
                            bool __atend = false);                              // 0x00A6BA60
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

template void vector<CIVCountedPointer<CManagedTextureData> >::_M_fill_insert(
    CIVCountedPointer<CManagedTextureData>*, unsigned int, const CIVCountedPointer<CManagedTextureData>&);

}
