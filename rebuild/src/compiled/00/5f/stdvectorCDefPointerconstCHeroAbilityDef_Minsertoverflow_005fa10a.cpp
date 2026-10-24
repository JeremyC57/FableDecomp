// std::vector<CDefPointer<const CHeroAbilityDef> >::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport grow-and-insert: allocate max(2*size, size+fill_len) slots, copy the prefix, add-ref fill_len copies of x, copy the suffix unless appending, free the old block
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FA10A, 176 bytes, `ret 14h`; caller: push_back 0x005FA3A5 (this chunk), used
//   by CTCInventoryAbilities::GetPrioritisedAbilityDefs 0x005F4B30.
// Callees: malloc 0x00BFEA0E (the inline allocate), __uninitialized_copy(first, last, result, const __false_type&)
//   0x005F79F7 and __uninitialized_fill_n(first, n, x, const __false_type&) 0x005F7A1D (landed; both `/Gr` `__fastcall`),
//   vector::_M_clear() 0x005F9F37 (landed; `_Destroy` of the old elements, then free).
// Label correction: the catalog `std_vector_resize_grow` is behavioural; the body is STLport 4.x `_M_insert_overflow` for
//   a class element -- the landed 0x005B3275 (vector<CDefPointer<const CTattooDef> >) template, byte-identical up to the
//   call targets: the one-element `_Copy_Construct` is the inline add-ref copy inside the placement null test.
// Shape: frame-pointer size TU (`#pragma optimize("y", off)` + `("s", on)`); every empty tag is homed in the high byte of
//   the `atend` slot (`lea eax,[ebp+1Bh]`) thanks to the same-TU helper bodies below; the copy ctor is `__forceinline`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
#include <new>

extern "C" void* __cdecl malloc(size_t size);       // 0x00BFEA0E

struct CDefPointeeBase {                             // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy() const;                    // slot 1
    mutable long m_refCount;                         // +0x04
};

struct CHeroAbilityDef : CDefPointeeBase { };

template <class T> class CDefPointer {
public:
    T* Object;                                       // +0x00
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer();                                  // out of line
};

typedef CDefPointer<const CHeroAbilityDef> CAbilityDefPointer;

namespace std {

struct __false_type { __false_type() { } };

// Same-TU stand-ins for the out-of-line helpers (their STLport bodies, tags unnamed and untouched): with them
// visible VC7.1 homes every empty tag over the dead `atend` byte, as retail.
#pragma auto_inline(off)
CAbilityDefPointer* __fastcall __uninitialized_copy(const CAbilityDefPointer* __first, const CAbilityDefPointer* __last,
                                                    CAbilityDefPointer* __result, const __false_type&)  // 0x005F79F7
{
    CAbilityDefPointer* __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        new (__cur) CAbilityDefPointer(*__first);
    return __cur;
}
CAbilityDefPointer* __fastcall __uninitialized_fill_n(CAbilityDefPointer* __first, size_t __n, const CAbilityDefPointer& __x,
                                                      const __false_type&)                             // 0x005F7A1D
{
    for (; __n > 0; --__n, ++__first)
        new (__first) CAbilityDefPointer(__x);
    return __first;
}
#pragma auto_inline(on)

template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> __forceinline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> struct allocator {
    __forceinline _Tp* allocate(size_t __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp*                                                _M_start;             // +0x00
    _Tp*                                                _M_finish;            // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, allocator<_Tp> >       _M_end_of_storage;    // +0x08
    size_t size() const { return size_t(_M_finish - _M_start); }
    void _M_clear();                                                          // 0x005F9F37
    __forceinline void _M_set(pointer __s, pointer __f, pointer __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_t __fill_len,
                            bool __atend = false);
};

template <class _Tp>
void vector<_Tp>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_t __fill_len,
                                     bool __atend)
{
    const size_t __old_size = size();
    const size_t __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = _M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CAbilityDefPointer>::_M_insert_overflow(CAbilityDefPointer*, const CAbilityDefPointer&,
                                                             const __false_type&, size_t, bool);

}