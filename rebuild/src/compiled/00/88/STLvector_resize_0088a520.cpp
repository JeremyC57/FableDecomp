// _STL::vector::resize(size_type new_size, const CBlendedEnvironmentTheme& x) -- shrink (inline erase: copy down + destroy the tail) or grow with a fill insert (500-byte blended environment themes)
// Leaf: ?resize@?$vector@VCBlendedEnvironmentTheme@NEnvironmentInternal@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088A520, 110 bytes, thiscall, `ret 8`.
// Identity: vector<NEnvironmentInternal::CBlendedEnvironmentTheme> (CArray<CBlendedEnvironmentTheme> derives from it).
//   Callers: CEnvironmentThemeSetDay::Build (0x0088844D; Ego_r.pdb CEnvironmentThemeSetDay::Themes is
//   CArray<CBlendedEnvironmentTheme>), CEnvironmentThemePriorityStackEntry::GetLevelTheme / BlendInLevel (0x008885DD,
//   0x008888C0) and 0x0088A64D. Ego_r CBlendedEnvironmentTheme is 524 bytes: TimeOfDay, Colours[19], Scalers[30] and
//   three two-CArray texture sets (16 -> 12 each in retail) = 500 = the 0x1F4 stride (0x10624DD3, `sar edx,5`).
//   Catalog "vector<CTCInventoryMap::CUIMiniMapMarker>::resize" is propagated.
// Callees: __copy(first, last, result, const random_access_iterator_tag&, int*) @ 0x00889830,
//   __destroy_aux(first, last, const __false_type&) @ 0x006B67A0 (ICF-shared), both __fastcall; _M_fill_insert
//   @ 0x0088A180 (thiscall).
// Shape: twin of 0x006B6D90 (inline erase, both tag temporaries in the dead n slot); the uncorrected reciprocal
//   multiply leaves the difference in edx (no `add edx,esi`).
class CRGBFloatColour {
public:
    float R, G, B, A;
};

class CIDrawEnvironmentTextureSet {           // retail 24 bytes
public:
    long* Textures[3];                        // +0x00  CArray<long> (STLport vector)
    float* Blends[3];                         // +0x0C  CArray<float>
};

namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme {              // retail 500 bytes
public:
    float TimeOfDay;                          // +0x000
    CRGBFloatColour Colours[19];              // +0x004
    float Scalers[30];                        // +0x134
    CIDrawEnvironmentTextureSet TextureSets[3];   // +0x1AC
};
}

namespace _STL {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator { public: allocator() { } };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

// out of line in retail (__fastcall: /Gr TU)
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);

template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, (NEnvironmentInternal::CBlendedEnvironmentTheme*)0);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last) {
        iterator __i = __copy_ptrs((const_pointer)__last, (const_pointer)this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);    // out of line in retail
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x) {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};

template void vector<NEnvironmentInternal::CBlendedEnvironmentTheme>::resize(size_type, const NEnvironmentInternal::CBlendedEnvironmentTheme&);
}