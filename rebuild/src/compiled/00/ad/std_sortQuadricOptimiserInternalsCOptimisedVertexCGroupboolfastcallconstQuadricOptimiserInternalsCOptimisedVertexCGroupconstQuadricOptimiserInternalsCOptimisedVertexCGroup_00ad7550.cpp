// std::sort(QuadricOptimiserInternals::COptimisedVertex::CGroup* first, QuadricOptimiserInternals::COptimisedVertex::CGroup* last, bool (__fastcall* comp)(const CGroup&, const CGroup&)) -- introsort loop to depth 2*lg(n), then the final insertion sort (inlined: threshold 16)
// Leaf: ??$sort@PAVCGroup@COptimisedVertex@QuadricOptimiserInternals@@P6I_NABV123@0@Z@std@@YIXPAVCGroup@COptimisedVertex@QuadricOptimiserInternals@@0P6I_NABV123@1@Z@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD7550, oracle length 115 bytes.
// Identity (naming wave 20, chunk 687): STLport 4.6 sort (stl/_algo.c) over the 8-byte COptimisedVertex::CGroup {BoneIndex, BoneWeight} with the
//   comparison FUNCTION pointer CGroup::CompareWeight 0x00AC6B40 -- the spelling the landed LimitGroupCount 0x00ACA5F0 (chunk 687, Ego_r.pdb)
//   binds; the callees __introsort_loop 0x00AD6F10 and __unguarded_insertion_sort_aux 0x00AD1CA0 are landed under this instantiation's spellings,
//   __insertion_sort 0x00AD5B50 under a non-template SKeyPair8 stand-in (DEFERRED). Was the descriptive Sort_SElement8_00ad7550 (one of 22 bodies).
// Retail bytes:  push ebx ; push edi ; mov ebx,edx ; mov edi,ecx ; cmp edi,ebx ; je E ; (n = (last - first) / 8) ; xor ecx,ecx ; cmp eax,1 ; je K ; L: sar eax,1 ; inc ecx ; cmp eax,1 ; jne L ; K: push [comp] ; push 0 ; mov edx,ebx ; mov ecx,edi ; call __introsort_loop ; (final insertion sort inlined, threshold 16) ...
// /Gr TU (__fastcall: first in ecx, last in edx, comp on the stack); `__lg` is the shift-count loop, its result int (the iterator difference).
namespace QuadricOptimiserInternals {
class COptimisedVertex { public:
    class CGroup { public:                              // 8 bytes (Ego_r.pdb nested class)
        unsigned long BoneIndex;
        float         BoneWeight;
        static bool __fastcall CompareWeight(const CGroup& a, const CGroup& b);   // 0x00AC6B40
    };
};
}
typedef QuadricOptimiserInternals::COptimisedVertex::CGroup CGroup;
typedef bool (__fastcall* PFNGroupLess)(const CGroup&, const CGroup&);
namespace std {
template <class _Iterator> struct iterator_traits;
template <class _Tp> struct iterator_traits<_Tp*> {
    typedef _Tp value_type;
};
const int __stl_threshold = 16;
template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1)
        ++__k;
    return __k;
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp);   // 0x00AD6F10
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);                                 // 0x00AD5B50
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp);          // 0x00AD1CA0
template <class _RandomAccessIter, class _Compare>
inline void __fastcall __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, (typename iterator_traits<_RandomAccessIter>::value_type*)0, __comp);   // _STLP_VALUE_TYPE
}
template <class _RandomAccessIter, class _Compare>
__forceinline void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)   // inlined by retail here (out of line in the CScriptManager sort 0x00CBDF70)
{
    if (__last - __first > __stl_threshold) {
        __insertion_sort(__first, __first + __stl_threshold, __comp);
        __unguarded_insertion_sort(__first + __stl_threshold, __last, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
}
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (typename iterator_traits<_RandomAccessIter>::value_type*)0, __lg(__last - __first) * 2, __comp);   // _STLP_VALUE_TYPE
        __final_insertion_sort(__first, __last, __comp);
    }
}
template void __fastcall sort<CGroup*, PFNGroupLess>(CGroup*, CGroup*, PFNGroupLess);
}
