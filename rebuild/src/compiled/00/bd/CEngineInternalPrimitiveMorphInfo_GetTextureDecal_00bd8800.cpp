// CEngineInternalPrimitiveMorphInfo::GetTextureDecal(long originalTextureIndex, long decalID) -- look up the texture decal registered for (original texture index, decal ID) in the morph info's sorted decal map; the decal, or null when there is none
// Leaf: GetTextureDecal
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BD8800, 80 bytes, thiscall, `ret 8`.
// Identity: descriptive name (no Ego_r.pdb name; no direct callers in retail). `this` is the morph info whose
//   TextureDecals map sits at +0 (Ego_r.pdb CEngineInternalPrimitiveMorphInfo); the same find is inlined after the
//   out-of-line CVectorMap::LowerBound 0x00BD8480 in CEnginePrimitiveManagerAnimatedMeshesBase::RenderMeshPrimitive
//   0x00BD31DE and CEnginePrimitiveManagerMeshBase::PreloadDiffuseTexture 0x00BEC03C (key = the material's
//   (DiffuseMapBankIndex, DecalID), i.e. CEngineTextureDecal's (OriginalTextureIndex, DecalID)). Catalog
//   "Texture_FindOrCreateCacheEntry" is wrong (nothing is created).
// Callees: CVectorMap<pair<long, long>, CEngineTextureDecal, ...>::LowerBound(const pair<long, long>&) 0x00BD8480.
// Shape: the key pair is built in the frame from the two arguments; the map find is STLport's `(it == end() ||
//   comp(key, *it)) ? end() : it` with end() read once after the call; the decal is &it->second (+8). LowerBound is an
//   instantiation of this TU (it sits just before this row), so its body is given here as a same-TU noinline stand-in:
//   seeing that it only reads the key, VC7.1 compares the found entry with the argument registers, not the frame copy.
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
    iterator LowerBound(const K& key);             // 0x00BD8480 (out of line)
    iterator Find(const K& key)
    {
        iterator it = LowerBound(key);
        return (it == end() || Compare(key, *it)) ? end() : it;
    }
};

#pragma auto_inline(off)
template <class K, class V, class C>
__declspec(noinline) typename CVectorMap<K, V, C>::iterator CVectorMap<K, V, C>::LowerBound(const K& key)
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
#pragma auto_inline(on)

typedef _STL::pair<long, long> TTextureDecalKey;
typedef CVectorMap<TTextureDecalKey, CEngineTextureDecal, CKeyPairCompareLess<TTextureDecalKey, CEngineTextureDecal> > TTextureDecalMap;

class CEngineInternalPrimitiveMorphInfo {
public:
    TTextureDecalMap TextureDecals;                // +0x00
    CEngineTextureDecal* GetTextureDecal(long originalTextureIndex, long decalID);
};

CEngineTextureDecal* CEngineInternalPrimitiveMorphInfo::GetTextureDecal(long originalTextureIndex, long decalID)
{
    TTextureDecalMap::iterator it = TextureDecals.Find(TTextureDecalKey(originalTextureIndex, decalID));
    return it != TextureDecals.end() ? &it->second : 0;
}