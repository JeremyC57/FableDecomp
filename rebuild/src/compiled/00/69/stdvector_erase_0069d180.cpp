// std::vector::erase(iterator first, iterator last) -- (vector<CGameCameraManager::CQuake >) STLport range erase of trivially-assigned records: copy [last, end()) down onto first, set the new end; returns first
// Leaf: ?erase@?$vector@VCQuake@CGameCameraManager@@V?$allocator@VCQuake@CGameCameraManager@@@std@@@std@@QAEPAVCQuake@CGameCameraManager@@PAV34@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069D180, 99 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity (proven): std::vector<CGameCameraManager::CQuake >::erase(CGameCameraManager::CQuake*, CGameCameraManager::CQuake*); evidence: CGameCameraManager::ResetCamera 0x0069A990 calls it with [this+0xEC] begin / end -- engine/CGameCameraManager.h EarthQuakes +0xEC, Ego_r.pdb vector<CGameCameraManager::CQuake> (48-byte records = the 0x30 stride), i.e. EarthQuakes.clear(); FableWin.pdb holds vector<CGameCameraManager::CQuake>::erase.
// Label: the manifest's "Vector_CompactElements" is behavioural / propagated (family J102: five range erases of 0x24- to 0x34-byte records;
//   retail has no identical-code folding, so each address is its own element type).
// Callees: none (the forward __copy is inline: a division by 0x30 (multiply by 0x2AAAAAAB), then one `rep movsd` of
//   12 dwords per element; _Destroy's loop over the trivially-destroyed tail folds away).
// Shape: STLport 4.6 _vector.h `erase(first, last)` over J092's helpers with __copy_ptrs / __copy `__forceinline` (plain inline: VC7.1 keeps __copy out of line).
#define CGameCameraManager CGameCameraManagerPdb
#include "engine/CGameCameraManager.h"
#undef CGameCameraManager
class C3DVector {                                          // 12 bytes (Ego_r.pdb)
public:
    float X;                                               // +0x00
    float Y;                                               // +0x04
    float Z;                                               // +0x08
};
class CGameCameraManager : public CGameCameraManagerPdb {  // engine/CGameCameraManager.h is data-only: the nested record is added here
public:
    class CQuake {                                         // 48 bytes (Ego_r.pdb; class key V from FableWin.pdb)
    public:
        C3DVector Position;                                // +0x00
        C3DVector Axis;                                    // +0x0C
        long Frame;                                        // +0x18
        float Amplitude;                                   // +0x1C
        float Duration;                                    // +0x20
        float Frequency;                                   // +0x24
        float Phase;                                       // +0x28
        bool Global;                                       // +0x2C
    };
};
namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                 const random_access_iterator_tag&, _Distance*)   // inline in retail (agent951 G040's device)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
__forceinline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator erase(iterator __first, iterator __last)
    {
        iterator __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
} // namespace std
typedef CGameCameraManager::CQuake TElement_0069d180;
template TElement_0069d180* std::vector<TElement_0069d180 >::erase(TElement_0069d180*, TElement_0069d180*);