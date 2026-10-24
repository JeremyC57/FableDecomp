// CVectorMap::LowerBound(const pair<long, long>& key) -- binary search of the morph info's sorted texture-decal map (28-byte entries keyed by (original texture index, decal ID)) for the first entry whose key is not below key
// Leaf: ?LowerBound@?$CVectorMap@U?$pair@JJ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BD8480, 106 bytes, thiscall, `ret 4`, the entry pointer (STLport iterator) in eax.
// Identity: Ego_r.pdb ?LowerBound@?$CVectorMap@U?$pair@JJ@std@@VCEngineTextureDecal@@V?$CKeyPairCompareLess@U?$pair@JJ@std@@VCEngineTextureDecal@@@@@@QAE...
//   (CEngineInternalPrimitiveMorphInfo::TextureDecals, the map at +0 of the morph info). Callers:
//   CEnginePrimitiveManagerAnimatedMeshesBase::RenderMeshPrimitive 0x00BD31DE, CEnginePrimitiveManagerMeshBase::
//   PreloadDiffuseTexture 0x00BEC03C (key (DiffuseMapBankIndex, DecalID) of the material), 0x00BD86C9 and the find
//   0x00BD8800. Catalog "std_lower_bound_TextureKey" is descriptive.
// Layout: CVectorMap = STLport vector<pair<pair<long, long>, CEngineTextureDecal> > {First, Last, EndOfStorage}, the empty
//   comparator +0x0C, Dirty +0x0D; CEngineTextureDecal is 20 bytes in retail (Ego_r 24, 16-byte Dinkumware vector).
// Shape: the CVectorMap bound of the landed 0x00BED2A0 (count = (Last - First) / 28 by `imul 0x92492493`; half =
//   count / 2 with `cdq ; sub ; sar`; `count -= half + 1` as `or -1 ; sub ; add`); the pair operator< inlines to
//   `cmp key.first, entry.first ; jg ; jl ; cmp entry.second, key.second ; jge` with key.first read once.
class CCompositeBlendDesc;

class CEngineTextureDecal {                        // Ego_r.pdb 24 bytes, retail 20 (12-byte STLport vector)
public:
    long                 OriginalTextureIndex;     // +0x00
    long                 DecalID;                  // +0x04
    CCompositeBlendDesc* OverlaidBlends[3];        // +0x08 vector<CCompositeBlendDesc> {start, finish, end}
};

namespace _STL {

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}

}   // namespace _STL

template <class K, class V> class CKeyPairCompareLess {
public:
    CKeyPairCompareLess() { }
    bool operator()(const _STL::pair<K, V>& entry, const K& key) const { return entry.first < key; }
    bool operator()(const K& key, const _STL::pair<K, V>& entry) const { return key < entry.first; }
};

template <class K, class V, class C> class CVectorMap {
public:
    typedef _STL::pair<K, V> value_type;
    typedef value_type* iterator;
    value_type* First;                             // +0x00 STLport vector<pair<K, V> >
    value_type* Last;                              // +0x04
    value_type* EndOfStorage;                      // +0x08
    C           Compare;                           // +0x0c
    bool        Dirty;                             // +0x0d
    iterator end() { return Last; }
    iterator LowerBound(const K& key);
};

template <class K, class V, class C>
typename CVectorMap<K, V, C>::iterator CVectorMap<K, V, C>::LowerBound(const K& key)
{
    value_type* first = First;
    int count = Last - First;
    C comp;
    while (count > 0)
    {
        int half = count / 2;
        value_type* middle = first + half;
        if (comp(*middle, key))
        {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}

typedef _STL::pair<long, long> TTextureDecalKey;
template class CVectorMap<TTextureDecalKey, CEngineTextureDecal, CKeyPairCompareLess<TTextureDecalKey, CEngineTextureDecal> >;