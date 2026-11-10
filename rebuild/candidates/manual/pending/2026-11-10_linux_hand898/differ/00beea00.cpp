// std::__adjust_heap(std::pair<std::pair<long,long>,CEngineTextureDecal>* first, int holeIndex, int len, std::pair<std::pair<long,long>,CEngineTextureDecal> val, CKeyPairCompareLess<std::pair<long,long>,CEngineTextureDecal> comp) -- STLport heap sift-down over the texture-decal vector map: move the larger child up until the hole reaches a leaf, then push the value back up
// Grade: DIFFER(266v266) -- one scheduling difference in the by-value copy of `val` for the __push_heap call: retail loads the key's two words before `push comp` and pipelines the 16 scalar bytes (ld0 ld1 st0 ld2 st1 ld3 st2 st3), VC7.1 loads each word just before its store; POD/STLport pair copy ctors, the template converting ctor and an explicit CEngineTextureDecal copy ctor were inert
// Leaf: ??$__adjust_heap@PAU?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@std@@HU12@V?$CKeyPairCompareLess@U?$pair@JJ@std@@VCEngineTextureDecal@@@@@std@@YIXPAU?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@0@HHU10@V?$CKeyPairCompareLess@U?$pair@JJ@std@@VCEngineTextureDecal@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEEA00, 266 bytes, __fastcall (first ecx, holeIndex edx), `ret 24h`. 1 grade used.
// Identity: callers are the landed __make_heap 0x00BEEE40 (declares this callee) and 0x00BEEEF6 in the CVectorMap<pair<long,long>,
//   CEngineTextureDecal> sort TU; element 0x1C bytes (pair<long,long> key, Ego_r CEngineTextureDecal with a 12-byte STLport vector).
// Callees: vector<CCompositeBlendDesc>::operator= 0x006CADE0, vector<CCompositeBlendDesc> copy ctor 0x005BB346, __push_heap 0x00BEE420,
//   free 0x00BFEA14 (the by-value value's blend vector).
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14
class CCompositeBlendDesc;
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector(const vector& __x);                              // 0x005BB346
    vector& operator=(const vector& __x);                   // 0x006CADE0
    ~vector() { if (_M_start) free(_M_start); }
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2>
inline bool __fastcall operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}
}   // namespace std

class CEngineTextureDecal {
public:
    long OriginalTextureIndex;                              // +0x00
    long DecalID;                                           // +0x04
    std::vector<CCompositeBlendDesc> OverlaidBlends;        // +0x08
};

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);   // 0x00BEE420

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}
}   // namespace std

typedef std::pair<std::pair<long, long>, CEngineTextureDecal> CDecalEntry;
typedef CKeyPairCompareLess<std::pair<long, long>, CEngineTextureDecal> CDecalEntryLess;
template void __fastcall std::__adjust_heap(CDecalEntry*, int, int, CDecalEntry, CDecalEntryLess);
