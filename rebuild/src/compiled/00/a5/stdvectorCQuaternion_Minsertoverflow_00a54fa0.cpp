// std::vector<CQuaternion>::_M_insert_overflow(CQuaternion* position, const CQuaternion& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport reallocating insert of 16-byte records: allocate old_size + max(old_size, n), copy the head, construct the n new records, copy the tail unless appending, free the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A54FA0, length 207 bytes (+1 int3 pad), `ret 14h` (__fastcall, five arguments).
// Callers: 0x00A552A1 (the fill-insert 0x00A55190) and 0x00A5548E (push_back). Callees: malloc 0x00BFEA0E / free
//   0x00BFEA14 (STLport's malloc allocator), 0x00A51BE0 __uninitialized_copy and 0x00A51C20 __uninitialized_fill_n of the
//   same 16-byte element (both `__fastcall`, the tag reference on the stack).
// Identity: vector<CQuaternion> (family pass 3 / agent952 H076: the caller 0x00A4EFC0 resizes its local
//   vector<CQuaternion> through 0x00A55190 / this function and assigns it with 0x00A54BB0, whose source's first element
//   goes to CQuaternion::IsIdentity; 0x00A51BE0 / 0x00A51C20 are that element's copy / fill). The element is copied
//   bitwise (Ego_r CQuaternion: X, Y, Z, W, no user copy constructor), whereas C2DLineF has one (??0C2DLineF@@QAE@ABV0@@Z),
//   and vector<C2DLineF>'s own helpers are 0x00A13490 / 0x00A134D0 (0x00A13800). Was modelled as C2DLineF; the catalog
//   label `Vector_Reallocate_C2DLineF_C3DClothQuad` was propagated from byte-identical bodies.
// Shape: STLport 4.x `_M_insert_overflow` (non-POD path) verbatim, as the landed 0x004ADB50 / this chunk's 0x00A43B20:
//   every `__false_type()` tag temporary homed on the bool `__atend` slot (same-TU `auto_inline(off)` stand-ins for the
//   two tag-taking COMDATs that never touch the tag), `max` by reference with old_size in the `push ecx` word, the n == 1
//   placement copy inlined (four dwords through ecx); `_M_clear`'s `_Destroy` over a trivially destructible element
//   inlines to nothing, leaving the null-tested free.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)
extern "C" void* __cdecl malloc(unsigned int);      // 0x00BFEA0E
extern "C" void __cdecl free(void*);                // 0x00BFEA14

class CQuaternion {                                  // 16 bytes (Ego_r: X, Y, Z, W)
public:
    float X, Y, Z, W;
};

namespace std {
struct __false_type { __false_type() { } };          // user constructor: no zero-initialised tag temporary

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
// Same-TU stand-ins for the STLport COMDATs 0x00A51BE0 / 0x00A51C20 (they never touch the tag).
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    for ( ; __first != __last; ++__first, ++__result)
        _Construct(&*__result, *__first);
    return __result;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp>
inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Tp>
inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _Tp>
class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                   // +0x00
    _Tp* _M_finish;                                  // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    protected:
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
    protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);
    public:
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                             unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    _Tp* __new_start = this->_M_end_of_storage.allocate(__len);
    _Tp* __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CQuaternion>::_M_insert_overflow(CQuaternion*, const CQuaternion&, const __false_type&, unsigned int, bool);
}