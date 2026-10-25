// std::vector::push_back(const CBlendedEnvironmentTheme& x) -- (vector<NEnvironmentInternal::CBlendedEnvironmentTheme>) STLport push_back of a 500-byte blended environment theme: copy-construct in place when there is room, else the out-of-line _M_insert_overflow
// Leaf: ?push_back@?$vector@VCBlendedEnvironmentTheme@NEnvironmentInternal@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088A350, 62 bytes, thiscall, `ret 4`.
// Identity: the manifest's "_Push_back_500byte_object" is descriptive, and the neighbouring catalog rows' "CFeatDef" is
//   propagated (Ego_r.pdb CFeatDef is 100 bytes). The 0x1F4-byte element is NEnvironmentInternal::CBlendedEnvironmentTheme
//   = { float TimeOfDay; CRGBFloatColour Colours[19]; float Scalers[30]; CIDrawEnvironmentTextureSet TextureSets[3]; }
//   (the landed constructor 0x008864A0 and operator= 0x006B5DF0: 4 + 0x130 + 0x78 + 3 * 0x18 = 500 bytes); its copy
//   constructor 0x008894A0 is the implicit memberwise one (the word, two `rep movsd` blocks, then the three texture
//   sets' two CArray copies each). The only caller 0x008887F7 is CEnvironmentThemePriorityStackEntry::BlendInLevel
//   0x008886E0; Ego_r.pdb names vector<NEnvironmentInternal::CBlendedEnvironmentTheme> (V: a class).
// Callees: CBlendedEnvironmentTheme::CBlendedEnvironmentTheme(const CBlendedEnvironmentTheme&) 0x008894A0,
//   vector::_M_insert_overflow(pos, x, __false_type, 1, true) 0x0088A070.
// Shape: STLport 4.6 `if (finish != end_of_storage) { _Construct(finish, x); ++finish; } else _M_insert_overflow(finish,
//   x, _IsPODType(), 1UL, true);` -- the placement new null-tests finish; the overflow is out of line with a same-TU
//   noinline stand-in body that never reads the `__false_type` tag, so VC7.1 homes the tag in x's argument slot
//   (`lea edx,[esp+10h]` after x is loaded).
inline void* operator new(unsigned int, void* p) { return p; }

namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme {                            // 0x1F4 bytes in retail (only the size and the copy matter here)
public:
    unsigned char Data[0x1F4];
    CBlendedEnvironmentTheme(const CBlendedEnvironmentTheme& other);   // 0x008894A0
};
}

namespace _STL {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    __declspec(noinline) void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                                 unsigned int __fill_len, bool __atend);   // 0x0088A070
    void push_back(const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::push_back(const _Tp& __x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, __x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
}

// Same-TU stand-in for the out-of-line overflow 0x0088A070 (its own row): never reads the tag.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                             unsigned int __fill_len, bool __atend)
{
    if (__atend && __fill_len == 1)
        this->_M_finish = __position + (&__x != 0);
}
#pragma auto_inline(on)

template void vector<NEnvironmentInternal::CBlendedEnvironmentTheme>::push_back(
    const NEnvironmentInternal::CBlendedEnvironmentTheme&);

} // namespace _STL