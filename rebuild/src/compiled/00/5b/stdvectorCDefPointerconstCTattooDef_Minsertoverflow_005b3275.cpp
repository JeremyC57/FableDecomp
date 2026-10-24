// std::vector<CDefPointer<const CTattooDef> >::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, unsigned int fill_len, bool atend) -- grow the tattoo def-pointer vector and insert fill_len add-ref'd copies of x
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B3275, 176 bytes, `ret 14h`.
// Element: CDefPointer<const CTattooDef> (4 bytes, Ego_r.pdb `CDefPointeeBase* Object`; the copy ctor adds a reference
// at pointee + 4 inline, the destructor is out of line). Callers: its push_back @ 0x005B3325 (called from
// CTCInventoryTrade::ApplyTattooCard 0x005AD39D, which pushes the hero's tattoos read by GetTattoos 0x0071DE20 and tests
// CTattooDef::Permanent `[ebx+3Dh]`), plus six inline push_backs at 0x0071D34F .. 0x0076BA5A (other CDefPointer
// vectors folded onto this body).
// Callees: malloc @ 0x00BFEA0E (the inline allocate), __uninitialized_copy(first, last, result, const __false_type&)
// @ 0x005B1479 and __uninitialized_fill_n(first, n, x, const __false_type&) @ 0x005B149F (both `/Gr` `__fastcall`),
// vector::_M_clear() @ 0x005B31EC (`_Destroy` through 0x005B26E9, then free 0x00BFEA14).
// Label: the catalog "Tattoo_ReallocateAndCopy" is descriptive: the body is STLport 4.x `_M_insert_overflow` for a
// class element, the 0x00594D36 / 0x00594880 template (this chunk) with the one-element `_Copy_Construct` the inline
// add-ref copy (`mov eax,[x] ; test eax,eax ; mov [edi],eax ; je ; inc [eax+4]` inside the placement null test). Every
// empty tag lives in the high byte of the `atend` slot (`lea eax,[ebp+1Bh]`, same-TU helper bodies below).
// Shape: frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`; the copy ctor is `__forceinline` (the size TU
// otherwise calls it out of line from `_Copy_Construct`, and the register choice flips: 172 v 176).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
#include <new>

extern "C" void* __cdecl malloc(size_t size);       // 0x00BFEA0E

struct CDefPointeeBase {                             // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long m_refCount;                                 // +0x04
};

class CTattooDef;

template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;                         // +0x00 (Ego_r.pdb)
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer();                                  // out of line (0x005B1B5E is its scalar deleting destructor)
};

typedef CDefPointer<const CTattooDef> CTattooDefPointer;

namespace std {

struct __false_type { __false_type() { } };

// Same-TU stand-ins for the out-of-line helpers (their STLport bodies, tags unnamed and untouched): with them
// visible VC7.1 homes every empty tag over the dead `atend` byte, as retail.
#pragma auto_inline(off)
CTattooDefPointer* __fastcall __uninitialized_copy(const CTattooDefPointer* __first, const CTattooDefPointer* __last,
                                                   CTattooDefPointer* __result, const __false_type&)  // 0x005B1479
{
    CTattooDefPointer* __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        new (__cur) CTattooDefPointer(*__first);
    return __cur;
}
CTattooDefPointer* __fastcall __uninitialized_fill_n(CTattooDefPointer* __first, size_t __n, const CTattooDefPointer& __x,
                                                     const __false_type&)                             // 0x005B149F
{
    for (; __n > 0; --__n, ++__first)
        new (__first) CTattooDefPointer(__x);
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
    void _M_clear();                                                          // 0x005B31EC
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

template void vector<CTattooDefPointer>::_M_insert_overflow(CTattooDefPointer*, const CTattooDefPointer&, const __false_type&,
                                                            size_t, bool);

}