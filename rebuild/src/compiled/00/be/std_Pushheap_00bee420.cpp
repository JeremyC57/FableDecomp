// std::_Push_heap(std::pair<std::pair<long,long>,CEngineTextureDecal>* first, int hole, int top, std::pair<std::pair<long,long>,CEngineTextureDecal> val, CKeyPairCompareLess<std::pair<long,long>,CEngineTextureDecal> pred) -- percolate a texture decal up a max-heap ordered by its (long,long) key, then free the by-value decal's blend vector
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x00BEE420, 192 bytes incl. int3 padding (/O2). Called from the matching _Adjust_heap
// (call site 0x00BEEAEC). Callees: vector<CCompositeBlendDesc>::operator= 0x006CADE0, free 0x00BFEA14.
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, rotated loop) over a 28-byte element
// (`imul eax,eax,1Ch`): std::pair<long,long> key +0, CEngineTextureDecal +8 (Ego_r.pdb names:
// OriginalTextureIndex, DecalID, vector<CCompositeBlendDesc> OverlaidBlends -- 20 bytes in retail, the
// STLport vector being 12, as in the landed __copy_backward 0x00BEE4E0 of this TU). The key compare is
// the lexicographic pair operator< (signed: `jg move ; jl done ; jge done` on the second word). The
// implicit operator= moves the four words and calls the out-of-line vector operator=; the by-value
// decal's vector is released in the callee (inline STLport ~_Vector_base: `if (_M_start) free`).
// /Gr: first=ecx, hole=edx; `ret 24h` = top + 28-byte pair + the empty comparator.
// Register shape: retail keeps the key's first word in ebp across the vector-assignment calls (loaded
// before the prologue finishes) and leaves `top` in its slot; the verbatim STLport body keeps `top` in
// ebp instead (185 bytes). The model reads key.first into a local once, compares through an (entry,
// key0, key) overload of the comparator (same lexicographic test), and drops the value through a named
// destination pointer, key.second before key.first (retail's store order and esi reuse).
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14
namespace N00bee420 {
struct CCompositeBlendDesc;
template <class T> struct CRetailVector {                 // STLport vector<T>: 12 bytes
    T* _M_start;                                          // +0
    T* _M_finish;                                         // +4
    T* _M_end_of_storage;                                 // +8
    CRetailVector& operator=(const CRetailVector& other); // 0x006CADE0 for CCompositeBlendDesc
    ~CRetailVector() { if (_M_start) free(_M_start); }
};
struct CEngineTextureDecal {                              // Ego_r.pdb names; 20 bytes in retail
    long OriginalTextureIndex;                            // +0x00
    long DecalID;                                         // +0x04
    CRetailVector<CCompositeBlendDesc> OverlaidBlends;    // +0x08
};
template<class K, class V> struct pair { K first; V second; };
template<class K, class V> inline bool operator<(const pair<K, V>& l, const pair<K, V>& r)
{
    return l.first < r.first || !(r.first < l.first) && l.second < r.second;
}
template<class K, class V> struct CKeyPairCompareLess {
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
    // (entry, key) form with the key's first word already in a local
    bool operator()(const pair<K, V>& entry, long key0, const K& key) const
    {
        return entry.first.first < key0 || !(key0 < entry.first.first) && entry.first.second < key.second;
    }
};
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    const long _Key0 = _Val.first.first;
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Key0, _Val.first); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    _RanIt _Dst = _First + _Hole;
    _Dst->first.second = _Val.first.second;
    _Dst->first.first = _Key0;
    _Dst->second = _Val.second;
}
typedef pair<long, long> Key;
typedef pair<Key, CEngineTextureDecal> Elem;
typedef CKeyPairCompareLess<Key, CEngineTextureDecal> Pred;
template void _Push_heap<Elem*, int, Elem, Pred>(Elem*, int, int, Elem, Pred);
}