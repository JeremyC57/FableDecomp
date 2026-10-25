// std::vector<CCharString>::vector(size_type n) -- STLport sized constructor: allocate n strings through the out-of-line _Vector_base(n, allocator()), then copy-fill them from a default-constructed CCharString
// Leaf: ??0?$vector@VCCharString@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057FCDD, 58 bytes, thiscall, `ret 4`, returns this; built in a
//   size-optimised frame-pointer TU (`push ebp ; mov ebp,esp`, `leave`, n re-read from its slot instead of cached
//   in edi) -- `#pragma optimize("s", on)` + `#pragma optimize("y", off)`.
// Identity: the manifest's "std::vector<ulong>::vector" (and the landed _Vector_base 0x004CD9D0's caller note) is
//   propagated: the fill 0x0044B150 copy-constructs CCharStrings (0x0099EC30) from a CCharString() local
//   (0x0099E4B0 / ~ 0x0099EAE0), so the element is CCharString (the 4-byte _Vector_base body is folded across
//   element types). Callers: CTCHeroMorph's GetRandomBeardName 0x0057B9D7 and six more.
// Callees: _Vector_base(unsigned int, const allocator&) 0x004CD9D0 (landed), CCharString::CCharString() 0x0099E4B0,
//   __uninitialized_fill_n(CCharString*, unsigned int, const CCharString&, const __false_type&) 0x0044B150 (landed,
//   `__fastcall`), ~CCharString 0x0099EAE0.
// Shape: STLport 4.6 `explicit vector(size_type n) : _Vector_base(n, allocator_type()) { _M_finish =
//   uninitialized_fill_n(_M_start, n, _Tp()); }` -- the empty allocator temporary and the `__false_type` tag both land
//   in the top byte of the LIVE `n` argument slot (`lea eax,[ebp+0Bh]`), which needs same-TU noinline stand-ins for
//   the two callees that never touch them; the fill gets the default string's address from the constructor's eax.
//   Under "s" VC7.1 declines to inline the `uninitialized_fill_n` forwarder, so it is `__forceinline` (4 grades:
//   speed caches n in edi; declaration-only callees home the empty temporaries in fresh locals).
#pragma optimize("s", on)
#pragma optimize("y", off)
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;                           // +0x00
    CCharString();                                          // 0x0099E4B0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

// Same-TU stand-in for the out-of-line fill 0x0044B150 (landed elsewhere): never touches the tag.
#pragma auto_inline(off)
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        new (__cur) _Tp(__x);
    return __cur;
}
#pragma auto_inline(on)

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    __declspec(noinline) _Vector_base(unsigned int __n, const _Alloc& __a);   // 0x004CD9D0 (same-TU stand-in below)
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

#pragma auto_inline(off)
template <class _Tp, class _Alloc>
_Vector_base<_Tp, _Alloc>::_Vector_base(unsigned int __n, const _Alloc& __a)
    : _M_start(0), _M_finish(0), _M_end_of_storage(_Alloc(), (_Tp*)0)
{
    _M_start = _M_end_of_storage.allocate(__n);
    _M_finish = _M_start;
    _M_end_of_storage._M_data = _M_start + __n;
}
#pragma auto_inline(on)

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(unsigned int __n);
};

template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>::vector(unsigned int __n)
    : _Vector_base<_Tp, _Alloc>(__n, _Alloc())
{
    this->_M_finish = uninitialized_fill_n(this->_M_start, __n, _Tp());
}

template vector<CCharString>::vector(unsigned int);

} // namespace std