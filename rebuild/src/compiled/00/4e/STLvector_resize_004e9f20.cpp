// _STL::vector::resize(size_type new_size) -- forward to resize(new_size, value_type()) for the music entries
// Leaf: ?resize@?$vector@VCMusicEntry@@V?$allocator@VCMusicEntry@@@_STL@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E9F20, 39 bytes, thiscall, `ret 4`; size-optimised TU.
// Identity: vector<CMusicEntry> (second TU): the only caller is the CMusicEntry vector transfer at 0x004E9DE4 (inside the landed CMusicEntry GFSerialiseVectorBinaryIn 0x004E9D71); the
//   callee 0x004E9F47 is the two-argument resize. Ego_r.pdb members: ID, Filename (CDefString), Gain, CacheToHD.
//   Byte twin of 0x0046B65A (the same template instantiated in a second TU). Catalog "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: vector::resize(size_type, const value_type&) @ 0x004E9F47.
// Pragma: `#pragma optimize("y", off)` + `#pragma optimize("s", on)`.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }`; CMusicEntry() : ID(0), Gain(0.0f), CacheToHD(false) with the
//   CDefString's -1: `and [ebp-10h],0 ; fldz ; or [ebp-0Ch],-1 ; fstp [ebp-8] ; ... mov byte [ebp-4],0`.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CDefString {
public:
    long Index;
    CDefString() : Index(-1) { }
};

class CMusicEntry {
public:
    unsigned long ID;
    CDefString Filename;
    float Gain;
    bool CacheToHD;
    CMusicEntry() : ID(0), Gain(0.0f), CacheToHD(false) { }
};

namespace _STL {
template <class _Tp> class allocator { public: allocator() { } };

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
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x004E9F47)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

template void vector<CMusicEntry>::resize(size_type);
}