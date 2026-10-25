// CAnimSet::Clear() -- empty the Anims and InternalAlphas vectors and zero CurrentAlpha / LastAlpha
// Leaf: ?Clear@CAnimSet@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00711520, length 96 bytes. Callee: memmove @ 0x00BFEAE6.
// Identity (FINDINGS chunk 390: the landed "ResetBuffers" -- CNavigatorOwner_00711520 with a propagated
//   CNearestBuildingWithProperty manifest label -- is CAnimSet::Clear): Ego_r.pdb names ?Clear@CAnimSet@@QAEXXZ;
//   struct_layouts CAnimSet {UpdateMode +0, vector<short> Anims +4, vector<CTransitionBlendAlpha> InternalAlphas +0x10
//   (8-byte records), ..., CurrentAlpha / LastAlpha = retail +0x2C / +0x30 with the 12-byte STLport vectors} = the
//   trivially copied vector at +4, the 8-byte-record vector at +0x10 and the two zeroed words; xrefs: the only caller is
//   CAnimateThing::ClearAllAnims 0x0070EC30 (call at 0x0070EC3D with `this` = CAnimateThing+0 = CurrentAnim.MainAnimSet),
//   which then clears the CCompleteAnimation lists at +0x54 / +0x58 and the DelayedAnimQueue at +0x60.
// Shape: STLport `vector::clear()` = `erase(begin(), end())` = `__copy_ptrs(end, _M_finish, begin, _TrivialAss())`:
//   short has a trivial assignment, so `__copy_trivial` (`cmp eax,eax` on the twice-read finish pointer, memmove + byte
//   count otherwise); CTransitionBlendAlpha takes STLport's default __false_type traits, so the random-access `__copy`
//   loop (`n = (last - first) >> 3 ; n > 0`, one word pair per pass through ebx). The finish pointers are stored back,
//   then the two floats take the loop's zero register (edi).
#include <stddef.h>
extern "C" void* __cdecl memmove(void* destination, const void* source, size_t size);   // 0x00BFEAE6

namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> struct __type_traits { typedef __false_type has_trivial_assignment_operator; };
template <> struct __type_traits<short> { typedef __true_type has_trivial_assignment_operator; };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
          ((const char*)__last - (const char*)__first);
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}

template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _TrivialAss;
    pointer _M_start;                                                   // +0x00
    pointer _M_finish;                                                  // +0x04
    _STLP_alloc_proxy<pointer, _Tp, _Alloc> _M_end_of_storage;          // +0x08
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss());
        _M_finish = __i;                                                // _Destroy(__i, _M_finish): trivial
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
}

enum EAnimUpdateMode { ANIM_UPDATE_MODE_0 };
struct CTransitionBlendAlpha {                           // Ego_r.pdb, 8 bytes
    float Current;
    float Last;
};
class CAnimateThing;

class CAnimSet {                                         // Ego_r.pdb names; 0x54 bytes in retail
public:
    void Clear();
    EAnimUpdateMode UpdateMode;                          // +0x00
    std::vector<short> Anims;                            // +0x04
    std::vector<CTransitionBlendAlpha> InternalAlphas;   // +0x10
    float CurrentPercentageComplete;                     // +0x1c
    float LastPercentageComplete;                        // +0x20
    float Speed;                                         // +0x24
    unsigned long FrameInternalAlphasUpdated;            // +0x28
    float CurrentAlpha;                                  // +0x2c
    float LastAlpha;                                     // +0x30
    float LastUpdateChange;                              // +0x34
    float ChangeAlphaBy;                                 // +0x38
    CAnimateThing* PAnimator;                            // +0x3c
};

void CAnimSet::Clear()
{
    Anims.clear();
    InternalAlphas.clear();
    CurrentAlpha = 0.0f;
    LastAlpha = 0.0f;
}
