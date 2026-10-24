// std::__uninitialized_fill_n<CEngineTextureDecal*,unsigned int,CEngineTextureDecal>(first, n, x, const __false_type&) -- placement-copy one engine texture decal (with its overlaid blend list) into n raw slots, return the end of the built range
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070AAF0, length 135 bytes. `__fastcall` (/Gr algorithm TU): first in ecx,
//         n in edx, the value + the tag reference on the stack, `ret 8`.
// Callees: malloc (CRT import thunk) @ 0x00BFEA0E (the copied blend list's storage), __uninitialized_copy<
//          CCompositeBlendDesc*>(first, last, result, __false_type) @ 0x005BAF28 (out of line; its manifest label
//          `_Uninit_copy<CArray<CRandomAppearanceMorph::CBodyPartMesh>*...>` is propagated).
// Identity: the manifest label "RandomAppearanceMorph_CopyElements" is propagated. The function sits in the CTCTextureDecal
//   TU (0x0070A3E0..0x0070B320); its caller 0x0070B240 is vector<CEngineTextureDecal>::_M_insert_overflow, reached from the
//   inline push_back of CTCTextureDecal::PeekEngineDecals (0x0070A570, call site 0x0070A6DC). Element: the Ego_r.pdb
//   CEngineTextureDecal {OriginalTextureIndex, DecalID, vector<CCompositeBlendDesc> OverlaidBlends} = 20 bytes retail
//   (12-byte STLport vector of 16-byte blend descs: `sar 4`).
// Shape: STLport 4.x `__uninitialized_fill_n(first, n, x, __false_type)` (`for (; n > 0; --n, ++cur) _Construct(&*cur, x)`,
//   unsigned count kept in the dead `push ecx` word, placement new keeps its null test); the implicit CEngineTextureDecal
//   copy constructor copies the two words and inlines the STLport `vector(const vector&)`: `_Vector_base(x.size(),
//   x.get_allocator())` (three zero stores, `n != 0 ? malloc(n * 16) : 0`, start/finish/end) then
//   `_M_finish = __uninitialized_copy(x._M_start, x._M_finish, _M_start, __false_type())` with the tag homed in the dead
//   value slot. One eax zero serves the null test and the zero stores and is re-materialised after the call.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (import thunk)
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() {} };

struct CRGBColour { unsigned char B, G, R, A; };
struct CCompositeBlendDesc {                    // Ego_r.pdb, 16 bytes
    unsigned long TextureIndex;                 // +0x00
    long          BlendType;                    // +0x04 ECompositeBlendType
    CRGBColour    DiffuseColour;                // +0x08
    CRGBColour    SpecularColour;               // +0x0C
    CCompositeBlendDesc(const CCompositeBlendDesc& other);   // 0x005BA48A
};

namespace std {
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);   // 0x005BAF28 for CCompositeBlendDesc
}

template <class _Tp> struct allocator {        // std::allocator (empty)
    allocator() {}
    allocator(const allocator&) {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Alloc> struct _STLP_alloc_proxy : _Alloc {   // the end-of-storage pointer
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) {}
};
template <class _Tp> struct _Vector_base {
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, allocator<_Tp> > _M_end_of_storage;
    _Vector_base(unsigned int __n, const allocator<_Tp>& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
};
template <class _Tp> struct vector : _Vector_base<_Tp> {   // STLport vector (12 bytes)
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    allocator<_Tp> get_allocator() const { return allocator<_Tp>(); }
    vector(const vector& __x) : _Vector_base<_Tp>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = std::__uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start, __false_type());
    }
};

struct CEngineTextureDecal {                    // Ego_r.pdb; 20 bytes retail
    long OriginalTextureIndex;                  // +0x00
    long DecalID;                               // +0x04
    vector<CCompositeBlendDesc> OverlaidBlends; // +0x08
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

namespace std {
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template CEngineTextureDecal* __fastcall __uninitialized_fill_n(CEngineTextureDecal*, unsigned int, const CEngineTextureDecal&, const __false_type&);
}