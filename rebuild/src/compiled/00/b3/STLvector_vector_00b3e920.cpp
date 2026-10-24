// _STL::vector::vector(unsigned int n) -- (vector<CTextureManagerPoolInfo>) STLport sized constructor: pointers nulled, n 20-byte pool infos malloc'd (none for zero), end of storage set, then the out-of-line __uninitialized_fill_n copies a default-constructed info (pixel format -1) into each slot
// Leaf: ??0?$vector@VCTextureManagerPoolInfo@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3E920, 94 bytes. __fastcall, `ret 4`, returns this.
// Identity: FableWin.pdb `??0?$vector@VCTextureManagerPoolInfo@@V?$allocator@VCTextureManagerPoolInfo@@@std@@@std@@QAE@I@Z`;
//   the only caller is CArray<CTextureManagerPoolInfo>::CArray(unsigned) 0x00B3EA40 (`??0?$CArray@VCTextureManagerPoolInfo@@@@QAE@I@Z`).
//   CTextureManagerPoolInfo is Ego_r's 20 bytes {Width, Height, CPixelFormat PixelFormat +8, bool UseMipmaps +0xC,
//   Count +0x10}; its default ctor only runs CPixelFormat() (FormatIndex = -1). The catalog label
//   "CTextureManagerPool_Initialize" is descriptive.
// Callees: malloc 0x00BFEA0E; STLport __uninitialized_fill_n(first, n, x, const __false_type&) 0x00B3E390 (`__fastcall`:
//   first/n in ecx/edx, &x and the tag pushed; the empty __false_type sits in the dead `n` argument slot). Its catalog
//   label "fill<CTextureManagerPoolInfo*, ...>" is propagated.
// Shape: STLport 4.6 `explicit vector(n) : _Vector_base(n, allocator_type()) { _M_finish = uninitialized_fill_n(_M_start,
//   n, _Tp()); }` -- the landed vector<CLightInfo>::vector(n) 0x0098CCC0 model with the fill out of line.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

class CPixelFormat {
public:
    long FormatIndex;                                   // +0x00
    CPixelFormat() : FormatIndex(-1) { }
};

class CTextureManagerPoolInfo {                         // 20 bytes
public:
    unsigned long Width;                                // +0x00
    unsigned long Height;                               // +0x04
    CPixelFormat  PixelFormat;                          // +0x08
    bool          UseMipmaps;                           // +0x0C
    unsigned long Count;                                // +0x10
};

struct __false_type { __false_type() { } };

namespace _STL {
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&);   // 0x00B3E390

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

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
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(unsigned int __n)
        : _Vector_base<_Tp, _Alloc>(__n, _Alloc())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, _Tp());
    }
};
}

template _STL::vector<CTextureManagerPoolInfo>::vector(unsigned int);