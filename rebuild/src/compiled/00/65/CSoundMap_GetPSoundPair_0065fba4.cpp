// CSoundMap::GetPSoundPair(const CCharString& name) const -- look the sound name's CRC up in the sorted sound map; the entry's sound pair, or null when absent
// Leaf: ?GetPSoundPair@CSoundMap
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065FBA4, 61 bytes, thiscall `pop ebp ; ret 4`. Name: Ego_r.pdb
// `?GetPSoundPair@CSoundMap@@QBEPBVCSoundPair@@ABVCCharString@@@Z` (name set); the landed callers CTCVillage::UpdateCrowds
// 0x006594BB and CTCDoor::PlaySound 0x00730A00 already call it by this name (10 call sites in all).
// Callees: CCharString::GetCRC() const @ 0x00404310 (the out-of-line copy), CVectorMap<unsigned long, CSoundPair>::
// LowerBound(const unsigned long&) const @ 0x0065E412 (landed; 12-byte entries).
// Label correction: the catalog "CVectorMap_LowerBound_Faction" is propagated.
// Layout: Ego_r.pdb CSoundMap: SoundMap +0x00 and CriteriaMap (CVectorMap<unsigned long, CDefString>) after it; retail
// CVectorMap is 0x10 bytes ({first, last, end of storage}, comparator) -- only SoundMap.Last (+4) is read.
// Shape: `crc = name.GetCRC(); it = SoundMap.Find(crc); if (it != end()) return &it->second; return 0;` with Find inline
// (`it == end() || Compare(key, *it) ? end() : it`, PDB `Find(const K&)`). The crc stays in edi for the compare while a
// frame copy in the dead name slot ([ebp+8]) feeds LowerBound's reference: that needs LowerBound's body in the TU
// (`auto_inline(off)`, the landed 0x0065E412 body) -- with a declaration only, VC7.1 re-reads the escaped key after the
// call. The `!=` test order gives retail's `je` to the null return.
// Opt: frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharString {
public:
    void* PStringData;
    unsigned long GetCRC() const;                         // 0x00404310 (out-of-line copy)
};

class CSoundPair {                                         // Ego_r.pdb, 8 bytes
public:
    long FirstSound;                                       // +0x00
    long LastSound;                                        // +0x04
};
class CDefString { public: long Index; };

template <class A, class B> struct pair {
    A first;
    B second;
};
template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const pair<K, V>& entry, const K& key) const { return entry.first < key; }
    bool operator()(const K& key, const pair<K, V>& entry) const { return key < entry.first; }
};

template <class K, class V, class C> class CVectorMap {
public:
    typedef pair<K, V> value_type;
    typedef const value_type* const_iterator;
    value_type* First;                                     // +0x00
    value_type* Last;                                      // +0x04
    value_type* EndOfStorage;                              // +0x08
    C           Compare;                                   // +0x0c (empty, padded)
    const_iterator end() const { return Last; }
    const_iterator LowerBound(const K& key) const;         // 0x0065E412
    __forceinline const_iterator Find(const K& key) const
    {
        const_iterator it = LowerBound(key);
        if (it == end() || Compare(key, *it))
            return end();
        return it;
    }
};

#pragma auto_inline(off)
template <class K, class V, class C>
typename CVectorMap<K, V, C>::const_iterator CVectorMap<K, V, C>::LowerBound(const K& key) const
{
    int count = Last - First;
    const value_type* first = First;
    C comp;
    while (count > 0)
    {
        int half = count / 2;
        const value_type* middle = first + half;
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

class CSoundMap {
public:
    CVectorMap<unsigned long, CSoundPair, CKeyPairCompareLess<unsigned long, CSoundPair> > SoundMap;      // +0x00
    CVectorMap<unsigned long, CDefString, CKeyPairCompareLess<unsigned long, CDefString> > CriteriaMap;   // +0x10
    const CSoundPair* GetPSoundPair(const CCharString& name) const;
};

const CSoundPair* CSoundMap::GetPSoundPair(const CCharString& name) const
{
    unsigned long crc = name.GetCRC();
    CVectorMap<unsigned long, CSoundPair, CKeyPairCompareLess<unsigned long, CSoundPair> >::const_iterator it = SoundMap.Find(crc);
    if (it != SoundMap.end())
        return &it->second;
    return 0;
}