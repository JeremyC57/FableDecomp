// std::vector<CEngineLightingManager::CThemeChannel>::resize(size_type new_size, const T& x) -- STLport resize: erase [begin() + new_size, end()) when shrinking, else fill-insert new_size - size() copies of x at the end
// Leaf: ?resize@?$vector@UCThemeChannel@CEngineLightingManager@@V?$allocator@UCThemeChannel@CEngineLightingManager@@@std@@@std@@QAEXIABUCThemeChannel@CEngineLightingManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b4a4c0, 92 bytes, thiscall, `ret 8`.
// Identity (proven): CEngineLightingManager::FrameStart 0x00B484B0 and CEnginePrimitiveManager::Initialise 0x00B482A0 call it; 112-byte stride = sizeof(CEngineLightingManager::CThemeChannel), whose __uninitialized_copy 0x00B49760 sits under its fill insert 0x00B4A2C0 (family L021); Ego_r.pdb names ?resize@?$vector@UCThemeChannel@CEngineLightingManager@@....
// Callees: __copy<T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x00B490F0 (__fastcall) and
//   vector::_M_fill_insert(iterator, size_type, const T&) 0x00B4A2C0 (thiscall), both out of line.
// Shape (family L041): size() by the 112-byte magic divide; erase(first, last) inline with __copy_ptrs forwarding to
//   the out-of-line __copy (its empty tag homed on the n argument slot, which needs __copy's same-TU noinline body),
//   trivial destructors, _M_finish = the copy's result.
class CRGBFloatColour {
public:
    float R, G, B, A;                                     // +0x00
};
class C4DVector {
public:
    float X, Y, Z, W;                                     // +0x00
};
class CEngineLightingManager {
public:
    struct CThemeChannel {                                 // Ego_r.pdb (struct), 112 bytes
        CRGBFloatColour DiffuseColour;                    // +0x00
        CRGBFloatColour AmbientColour;                    // +0x10
        CRGBFloatColour BacklightColour;                  // +0x20
        CRGBFloatColour ReflectionColour;                 // +0x30
        CRGBFloatColour FogColour;                        // +0x40
        float FogStartZ;                                  // +0x50
        float FogEndZ;                                    // +0x54
        float ShadowFactor;                               // +0x58
        float FadedShadowFactor;                          // +0x5C
        C4DVector FogTransform;                           // +0x60
    };
};
namespace std {
struct __false_type { };
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)        // 0x00B490F0
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {                                             // STLport 4.6, 12 bytes
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _Tp* _M_end_of_storage;                               // +0x08
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x);
protected:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // 0x00B4A2C0
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::resize(size_type __new_size, const _Tp& __x)
{
    if (__new_size < size())
        erase(begin() + __new_size, end());
    else
        insert(end(), __new_size - size(), __x);
}
}
template void std::vector<CEngineLightingManager::CThemeChannel >::resize(unsigned int, const CEngineLightingManager::CThemeChannel&);