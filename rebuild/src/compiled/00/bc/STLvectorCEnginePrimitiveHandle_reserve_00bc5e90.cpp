// std::vector<CEnginePrimitiveHandle>::reserve(size_type n) -- grow the handle vector's capacity to n: copy the handles into new storage (adding references) and release the old block, or just allocate when empty
// Leaf: ?reserve@?$vector@VCEnginePrimitiveHandle@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC5E90, 151 bytes, thiscall, `ret 4`.
// Identity: callers 0x00BC5AF2 / 0x00BC64E2; the out-of-line copy is the landed __uninitialized_copy<CEnginePrimitiveHandle*>
//   0x004400C0 (8-byte elements with vptr 0x0122F598) and the clear 0x00440A70 destroys each handle through its virtual
//   destructor before freeing the block. Catalog "std_vector_reserve_with_copy" is descriptive.
// Callees: malloc @ 0x00BFEA0E (allocate(n), both arms), __uninitialized_copy(first, last, result, const __false_type&)
//   0x004400C0 (/Gr-style __fastcall; the tag homed in the dead `n` slot), _M_clear() 0x00440A70.
// Shape: STLport 4.x `if (capacity() < n) { old_size = size(); if (_M_start) { tmp = _M_allocate_and_copy(n, _M_start,
//   _M_finish); _M_clear(); } else tmp = allocate(n); _M_set(tmp, tmp + old_size, tmp + n); }` with
//   _M_allocate_and_copy inline; the finish pointer parks in the `push ecx` frame word across the malloc call; the
//   copy is a same-TU noinline stand-in so the tag temporary shares the live `n` slot as in retail.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    long RefCount;                                     // +0x04
};

class CIVCountedPointer {                              // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* P;
    CIVCountedPointer(const CIVCountedPointer& x) : P(x.P) { if (P) ++P->RefCount; }
};

class CBaseClass {                                     // vtable 0x01231710
public:
    virtual ~CBaseClass();
};

class CEnginePrimitiveHandle : public CBaseClass {     // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer Data;                            // +0x04
};

namespace std {

struct __false_type { __false_type() { } };

// Same-TU stand-in for the out-of-line __uninitialized_copy 0x004400C0 (the landed body), compiled under
// auto_inline(off) + noinline: retail's whole-program build knew the tag is never read, so the tag temporary may sit
// on the still-live `n` argument slot. Only reserve is graded.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                                  const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        new (__cur) CEnginePrimitiveHandle(*__first);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

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
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    protected:
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    public:
    protected:
    void _M_clear();                                   // 0x00440A70
    public:
    protected:
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
    void reserve(size_type __n);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::reserve(size_type __n)
{
    if (capacity() < __n) {
        const size_type __old_size = size();
        pointer __tmp;
        if (this->_M_start) {
            __tmp = _M_allocate_and_copy(__n, this->_M_start, this->_M_finish);
            _M_clear();
        } else {
            __tmp = this->_M_end_of_storage.allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}

template void vector<CEnginePrimitiveHandle>::reserve(size_type);

}   // namespace std