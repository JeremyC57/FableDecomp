// std::sort_heap(CDegradableInfo* first, CDegradableInfo* last, CDegradableDef::CIsHealth1GTHealth2 comp) -- order a heap of degradable stages by health by repeatedly popping the front to the back
// Flags: /Gr
// Leaf: sort_heap
// Retail: Fable.exe @ 0x0067F040, 152 bytes of code (/O2). Callee: STLport __adjust_heap<CDegradableInfo*,int,
// CDegradableInfo,CDegradableDef::CIsHealth1GTHealth2> 0x0067E8F0 (`__fastcall`: first ecx, hole edx, the
// count, the 28-byte value and the empty comparator on the stack; it compares HealthPercentage with `fcomp`
// -- its catalog label `_Adjust_heap<CDebugBox*,...>` is propagated).
// Shape: STLport `while (last - first > 1) pop_heap(first, last--, comp);` with pop_heap -> __pop_heap_aux ->
// __pop_heap inline: `value = *(last - 1)` into the 28-byte frame local, `*(last - 1) = *first`, then
// `__adjust_heap(first, 0, (last - 1) - first, value, comp)` with the value block-copied into the argument
// area. CDegradableInfo (Ego_r.pdb, 28 bytes: HealthPercentage, CEngineGraphic, SmashParticleEmitter,
// BlocksNavigation) is a POD, so every copy is `rep movsd` x7; `last - first` is the signed divide by 28
// (`imul 92492493h`), and `last--` folds into the byte count kept in ebx.
// Label correction: catalog `_Sort_heap<CDegradableInfo*,CDegradableDef::CIsHealth1GTHealth2>` is the Ego_r
// (Dinkumware) spelling; retail is STLport sort_heap.
namespace N0067f040 {
struct CEngineGraphic {                                   // 14 bytes (+2 pad)
    unsigned long BankIndex;                              // +0x0
    float AnimStep;                                       // +0x4
    float RenderSizeX;                                    // +0x8
    bool AdditiveAlpha;                                   // +0xC
    unsigned char Type;                                   // +0xD
};
struct CDegradableInfo {                                  // 0x1c bytes
    float HealthPercentage;                               // +0x00
    CEngineGraphic EngineGraphic;                         // +0x04
    long SmashParticleEmitter;                            // +0x14
    bool BlocksNavigation;                                // +0x18
};
struct CDegradableDef {
    struct CIsHealth1GTHealth2 {
        bool operator()(const CDegradableInfo& a, const CDegradableInfo& b) const { return a.HealthPercentage > b.HealthPercentage; }
    };
};
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(const _Tp*) { return (int*)0; }
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x0067E8F0
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
__forceinline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
template <class _RandomAccessIterator, class _Tp, class _Compare>
__forceinline void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, __distance_type(__first));
}
template <class _RandomAccessIterator, class _Compare>
__forceinline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, __value_type(__first), __comp);
}
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}
template void __fastcall sort_heap<CDegradableInfo*, CDegradableDef::CIsHealth1GTHealth2>(CDegradableInfo*, CDegradableInfo*, CDegradableDef::CIsHealth1GTHealth2);
}