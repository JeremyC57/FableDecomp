// std::vector::vector(const vector&) -- STLport copy constructor of a vector of 8-byte elements: base nulled, size() elements malloc'd (none for zero), then each element placement-copied from the source
// Leaf: ??0?$vector@VCMorphSetTextureMorph@CMorphSet@CRandomAppearanceMorph@@
// Retail 0x0086B2A0, 0x76 bytes (+ int3 padding to 0x80), thiscall `ret 4`. Callee: malloc @ 0x00BFEA0E (the CRT import
// thunk STLport's allocator<T>::allocate reaches; `__cdecl`, `add esp,4`).
// Callers (8): 0x0086C798 (the out-of-line CArray<CMorphSetTextureMorph> copy constructor 0x0086C790: `vector(x)` then
// `return this`), 0x0086D48F (_Construct<CMeshTextureSet> 0x0086D480: MeshIndex dword, then this on +4), 0x0086D4BF (its
// __uninitialized_copy 0x0086D4A0, 16-byte stride), 0x0086D4FF (its __uninitialized_fill_n), 0x0086CEA2, 0x0086E33E,
// 0x0086E7E2, 0x0086E855 -- the CRandomAppearanceMorph container family (0x0086A9D0..0x0086E8xx).
// Element: CRandomAppearanceMorph::CMorphSet::CMorphSetTextureMorph (Ego_r: long OriginalTexture +0, long NewTexture +4;
// CMeshTextureSet = long MeshIndex + CArray<CMorphSetTextureMorph> TextureMorphs, retail 0x10 -- the 16-byte {dword,
// vector} element whose copy calls this). STLport 4's default __type_traits make every user class non-POD, so the copy
// is the __false_type placement-new loop with its null test.
// Shape: STLport `vector(const vector& x) : _Vector_base(x.size(), x.get_allocator()) { _M_finish =
// __uninitialized_copy(x._M_start, x._M_finish, _M_start, __false_type()); }` with _Vector_base(n, a) = null the three
// pointers, `_M_start = allocate(n)` (`n != 0 ? malloc(n * 8) : 0`), `_M_finish = _M_start`, `_M_end_of_storage =
// _M_start + n`; the copy loop re-reads x's pointers.
#include <new>

extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

class CRandomAppearanceMorph
{
public:
    class CMorphSet
    {
    public:
        class CMorphSetTextureMorph
        {
        public:
            long OriginalTexture;                    // +0x00
            long NewTexture;                         // +0x04
        };
    };
};

typedef CRandomAppearanceMorph::CMorphSet::CMorphSetTextureMorph TMorphSetTextureMorph;

namespace std {
struct __false_type { };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp> class allocator
{
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc
{
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base
{
public:
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;                                   // +0x00
    _Tp* _M_finish;                                  // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc>
{
public:
    typedef _Vector_base<_Tp, _Alloc> _Base;
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    _Alloc get_allocator() const { return _Alloc(); }
    vector(const vector<_Tp, _Alloc>& __x)
        : _Base(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start, __false_type());
    }
};
}

template std::vector<TMorphSetTextureMorph, std::allocator<TMorphSetTextureMorph> >::vector(
    const std::vector<TMorphSetTextureMorph, std::allocator<TMorphSetTextureMorph> >&);