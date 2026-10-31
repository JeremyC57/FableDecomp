// std::__pop_heap(pair<pair<long,long>,CEngineTextureDecal>* first, .. last, .. result, value, CKeyPairCompareLess comp, int*) -- STLport pop_heap core: *result = *first, then sift `value` down from the root through __adjust_heap
// Grade: DIFFER(147v147) -- scheduling only: in the by-value copy of `value` retail loads value.first.second into edx before `sub esp,1Ch` (two-deep load pipeline), ours after
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEEDA0, 147 bytes, __fastcall (/Gr), `ret 28h`. Callers: the sort_heap/partial_sort loops
//   0x00BEEF7A / 0x00BEF16A / 0x00BEF402 / 0x00BEF7C8. Identity: reloc_bindings ??$__pop_heap@PAU?$pair@U?$pair@JJ@std@@
//   VCEngineTextureDecal@@@std@@U12@V?$CKeyPairCompareLess@U?$pair@JJ@std@@VCEngineTextureDecal@@@@H@std@@YIX..@Z; the catalog
//   label `_Cons_val<pair<CCharString,C3DVector>>` is propagated garbage (the body copies 0x1C-byte pairs and divides by 28).
// Callees: vector<CCompositeBlendDesc>::operator= 0x006CADE0, vector<CCompositeBlendDesc> copy ctor 0x005BB346, __adjust_heap
//   0x00BEEA00 (same instantiation), free 0x00BFEA14 (the callee-owned `value`'s vector storage).
// Layout: Ego_r.pdb CEngineTextureDecal (OriginalTextureIndex +0, DecalID +4, OverlaidBlends +8, STLport 12-byte vector).
// Tried: real STLport pair template with its user copy ctor (this file), the earlier POD pair halves and member-wise copy
//   ctors; QFE 4035 compiler. Same residue as 0x00978B90. Next: lane gap: by-value struct-argument copy scheduling.
// Flags: /Gr
// Leaf: ??$__pop_heap@PAU?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@std@@
extern "C" void __cdecl free(void* block);       // 0x00BFEA14
class CCompositeBlendDesc;
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector(const vector& __x);                   // 0x005BB346
    ~vector() { if (_M_start) free(_M_start); }
    vector& operator=(const vector& __x);        // 0x006CADE0
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}
class CEngineTextureDecal { public:
    long OriginalTextureIndex;                   // +0x00
    long DecalID;                                // +0x04
    std::vector<CCompositeBlendDesc> OverlaidBlends;   // +0x08
};
template <class K, class V> class CKeyPairCompareLess { public: };
namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00BEEA00

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
typedef pair<pair<long, long>, CEngineTextureDecal> _DecalPair;
template void __pop_heap<_DecalPair*, _DecalPair, CKeyPairCompareLess<pair<long, long>, CEngineTextureDecal>, int>(
    _DecalPair*, _DecalPair*, _DecalPair*, _DecalPair, CKeyPairCompareLess<pair<long, long>, CEngineTextureDecal>, int*);
}
