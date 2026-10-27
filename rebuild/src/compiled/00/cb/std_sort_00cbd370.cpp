// std::sort(std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo >* first, std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo >* last, CKeyPairCompareLess<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo > comp) -- STLport sort: when the range is not empty, introsort it with depth limit 2 * floor(log2(n)), then finish with the final insertion sort
// Leaf: ??$sort@PAU?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@V?$CKeyPairCompareLess@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@@@std@@YIXPAU?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@0@0V?$CKeyPairCompareLess@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00cbd370, 83 bytes, /Gr __fastcall (first in ecx, last in edx, comp on the stack), `ret 4`.
// Identity (proven): bound (reloc_bindings) ??$sort@PAU?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@V?$CKeyPairCompareLess@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@@@std@@YIX...@Z; callers NScript::CRuntimeScriptedEntityPersist::SetEntityPersistScript 0x00CB82F0 / LoadFromGameStateScript 0x00CB8440 and the CVectorMap Sort 0x00CBD5B0.
// Callees: __introsort_loop<T*, T, int, Compare>(first, last, (T*)0, depth, comp) 0x00CBCA50 and
//   __final_insertion_sort<T*, Compare>(first, last, comp) 0x00CBC2B0 (both __fastcall, out of line).
// Shape (family L050): n by the 24-byte magic divide, the inline __lg loop (`sar eax,1 ; inc ecx` until 1), depth
//   `lea eax,[ecx+ecx]`; the 4-byte comparator copy re-pushed for both calls.
class CCharString {
public:
    void* PStringData;                                    // +0x00
};
namespace NScript {
class CRuntimeScriptedEntityPersist {
public:
    class CPersistInfo {                                   // Ego_r.pdb, 16 bytes
    public:
        unsigned __int64 UID;                             // +0x00
        CCharString PersistScript;                        // +0x08
    };
};
}
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess {    // empty, passed by value
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};
namespace std {
template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1)
        ++__k;
    return __k;
}
template <class _Tp> inline _Tp* __value_type(_Tp*) { return (_Tp*)0; }
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp);                                          // 0x00CBCA50
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00CBC2B0
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, __value_type(__first), __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}
}
template void __fastcall std::sort(std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo >*, std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo >*, CKeyPairCompareLess<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo >);