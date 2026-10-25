// _STL::vector::resize(size_type new_size, const CThemeChannel& x) -- shrink (inline erase: copy down + destroy the tail) or grow with a fill insert (496-byte environment theme channels)
// Leaf: ?resize@?$vector@UCThemeChannel@CIDrawEnvironmentParams@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B6D90, 112 bytes, thiscall, `ret 8`.
// Identity: vector<CIDrawEnvironmentParams::CThemeChannel> (FableWin.pdb mangles CThemeChannel as a struct). Callers:
//   CEnvironment::RenderUpdate (0x006B438D: `ThemeChannels.resize(6, CThemeChannel())`, the temporary built by
//   CThemeChannel::CThemeChannel 0x006B5FB0) and the one-argument resize at 0x006B6EC0 (0x1F0-byte frame temporary).
//   Ego_r.pdb CThemeChannel is 520 bytes: Colours[19], Scalers[30] and three CIDrawEnvironmentTextureSet (two CArrays
//   each, 16 -> 12 in retail STLport), so 424 + 3*24 = 496 = the 0x1F0 stride (0x84210843 + `add`, `sar edx,8`).
//   Catalog "vector<CTCInventoryMap::CUIMiniMapMarker>::resize" is propagated.
// Callees: __copy(first, last, result, const random_access_iterator_tag&, int*) @ 0x006B65C0 and
//   __destroy_aux(first, last, const __false_type&) @ 0x006B6610 (both __fastcall, out of line), _M_fill_insert
//   @ 0x006B6B30 (thiscall).
// Shape: STLport 4.6 `resize(n, x)` with `erase(begin() + n, end())` INLINED: `__copy_ptrs(last, _M_finish, first,
//   __false_type())` -> `__copy(..., random_access_iterator_tag(), (int*)0)` (first = last = _M_finish in ecx/edx),
//   then `_Destroy(i, _M_finish)` -> `__destroy_aux(i, _M_finish, __false_type())`, `_M_finish = i`. Both empty tag
//   temporaries live in the dead n argument slot (`lea edx,[esp+14h]` / `lea eax,[esp+10h]`); the tags need user
//   constructors or VC zero-stores them. The insert arm keeps size() and the first _M_finish load (no re-read).
class CRGBFloatColour {
public:
    float R, G, B, A;
};

class CIDrawEnvironmentTextureSet {           // retail 24 bytes
public:
    long* Textures[3];                        // +0x00  CArray<long> (STLport vector)
    float* Blends[3];                         // +0x0C  CArray<float>
};

class CIDrawEnvironmentParams {
public:
    struct CThemeChannel {                    // retail 496 bytes
        CRGBFloatColour Colours[19];          // +0x000
        float Scalers[30];                    // +0x130
        CIDrawEnvironmentTextureSet TextureSets[3];   // +0x1A8
    };
};

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
    __destroy(__first, __last, (CIDrawEnvironmentParams::CThemeChannel*)0);
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

template void vector<CIDrawEnvironmentParams::CThemeChannel>::resize(size_type, const CIDrawEnvironmentParams::CThemeChannel&);
}