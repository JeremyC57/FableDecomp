// std::vector<CDefPointer<const CHeroAbilityDef> >::push_back(const CDefPointer<const CHeroAbilityDef>& x) -- STLport append: add-ref copy into the spare slot, else grow through _M_insert_overflow
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FA3A5, 56 bytes, `ret 4`; callers: four sites in
//   CTCInventoryAbilities::GetPrioritisedAbilityDefs 0x005F4B30 (landed; it declares this push_back at 0x005FA3A5).
// Callee: _M_insert_overflow(position, x, const __false_type&, fill_len, atend) 0x005FA10A (this chunk).
// Label correction: the catalog `Vector_PushBack_Refcounted` is behavioural; the body is STLport
//   `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, x); ++_M_finish; }
//   else _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);`.
// Shape: frame pointer kept in a size TU (`#pragma optimize("y", off)` + `("s", on)`); `this` stays in ecx (no call on the
//   fast path); the inline CDefPointer copy constructor stores the pointer before its null test and add-refs with
//   `inc dword ptr [eax+4]`; the `__false_type` temporary is homed in the high byte of x's slot (`lea eax,[ebp+0Bh]`).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
#include <new>

class CDefPointeeBase { public:                             // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy() const;                    // slot 1
    mutable long m_refCount;                         // +0x04
};

class CHeroAbilityDef : public CDefPointeeBase { public: };

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

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> struct allocator { };

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
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_t __fill_len,
                            bool __atend = false);                            // 0x005FA10A
    void push_back(const _Tp& __x);
};

template <class _Tp>
void vector<_Tp>::push_back(const _Tp& __x)
{
    if (_M_finish != _M_end_of_storage._M_data) {
        _Construct(_M_finish, __x);
        ++_M_finish;
    }
    else
        _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
}

template void vector<CAbilityDefPointer>::push_back(const CAbilityDefPointer&);

}