// std::vector<pair<unsigned long, CTextureMorph> >::_M_allocate_and_copy(unsigned int n, const value_type* first, const value_type* last) -- STLport: malloc n 12-byte texture-morph entries (none for zero), copy-construct [first, last) into them, return the new block
// Leaf: ??$_M_allocate_and_copy@
//
// Retail 0x00673F90, 86 bytes (+ int3 padding to 96), thiscall `ret 0Ch` (n, first, last; `this` unused).
// Callee: malloc @ 0x00BFEA0E (STLport's __malloc_alloc behind allocator<T>::allocate).
// Caller: 0x00674232 in vector<pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> >::operator= 0x006741E0, the
//   vector of CRandomAppearanceMorph::GeneralTextureMorphs (landed CRandomAppearanceMorph::operator= 0x00675B60) --
//   CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph> keeps its entries as pair<key, value>.
// Layout: Ego_r.pdb CRandomAppearanceMorph::CTextureMorph = { ulong NewTexture; ulong GroupID; } (8 bytes), so the pair
//   is 12 bytes (`lea eax,[eax+eax*2] ; shl eax,2`).
// Shape: STLport 4.x _vector.h member template `_M_allocate_and_copy(n, first, last)`: `result = allocate(n)`
//   (n ? malloc(n * 12) : 0) then `__uninitialized_copy(first, last, result, __false_type())` (STLport has no POD
//   traits for a pair) with _Construct's placement null test and the pair's copy inline (three dword moves); the copy
//   helper needs __forceinline (plain inline stays a call, 60v86); speed TU
//   (`add esp,4` after malloc, `mov edi,edi` loop-head pad, destination as `result - first` delta).
// Label: catalog "Vector_AllocateTriplets" (descriptive).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CRandomAppearanceMorph {
public:
    class CTextureMorph {                                   // 8 bytes
    public:
        unsigned long NewTexture;                           // +0x00
        unsigned long GroupID;                              // +0x04
    };
};

namespace std {
struct __false_type { };

template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair& p) : first(p.first), second(p.second) {}
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    struct : _Alloc { _Tp* _M_data; } _M_end_of_storage;
    template <class _ForwardIterator>
    _Tp* _M_allocate_and_copy(unsigned int __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        _Tp* __result = _M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
};
}

typedef std::pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> CTextureMorphPair;
template CTextureMorphPair* std::vector<CTextureMorphPair>::_M_allocate_and_copy<const CTextureMorphPair*>(
    unsigned int, const CTextureMorphPair*, const CTextureMorphPair*);