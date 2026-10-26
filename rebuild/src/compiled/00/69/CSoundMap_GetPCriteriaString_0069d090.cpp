// CSoundMap::GetPCriteriaString(const CCharString& name) const -- look the name's CRC up in the sorted criteria map; the entry's def string, or null when absent
// Leaf: ?GetPCriteriaString@CSoundMap
//
// Retail 0x0069D090, 98 bytes (+ int3 padding to 112), thiscall `ret 4`.
// Callees: CCRC::Calc(seed, data, length) @ 0x004014A0 (the inline CCharString::GetCRC), CVectorMap<unsigned long,
//   CDefString>::LowerBound(const unsigned long&) const @ 0x0069C740 (landed; 8-byte entries).
// Callers: PlayCameraSoundThroughObservedThing 0x00698AC8 (the camera def's sound map, after GetPSoundPair misses),
//   0x00780E5B (CTCFlammable, its def's sound map at +0x28).
// Shape: the twin of the landed CSoundMap::GetPSoundPair 0x0065FBA4 on CriteriaMap (+0x10): `crc = name.GetCRC(); it =
//   CriteriaMap.Find(crc); if (it != CriteriaMap.end()) return &it->second; return 0;` -- here a speed TU with
//   GetCRC inline (length first, then the data / shared "" 0x0129AAF4), the crc kept in edi and homed in the dead name
//   slot for LowerBound's reference (LowerBound's body in the TU under auto_inline(off), as the landed twin), end()
//   re-read for the final compare.
// Name: Ego_r.pdb `?GetPCriteriaString@CSoundMap@@QBEPBVCDefString@@ABVCCharString@@@Z`; catalog "SoundMap_FindByCRC"
//   is descriptive.
class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
};
class CCharStringData { public:
    char*         PData;                                    // +0x00 CBasicString<char>::PData
    unsigned long StringLength;                             // +0x04
};
extern const char g_EmptyString[];                          // 0x0129AAF4
class CCharString {
public:
    CCharStringData* PStringData;
    const char* GetPString() const { return PStringData == 0 ? g_EmptyString : PStringData->PData; }
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    unsigned long GetCRC() const { return CCRC::Calc(0, GetPString(), GetLength()); }
};

class CSoundPair {                                          // Ego_r.pdb, 8 bytes
public:
    long FirstSound;                                        // +0x00
    long LastSound;                                         // +0x04
};
class CDefString { public: long Index; };

template <class A, class B> struct pair {
    A first;
    B second;
};
template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const pair<K, V>& entry, const K& key) const { return entry.first < key; }
    bool operator()(const K& key, const pair<K, V>& entry) const { return key < entry.first; }
};

template <class K, class V, class C> class CVectorMap {
public:
    typedef pair<K, V> value_type;
    typedef const value_type* const_iterator;
    value_type* First;                                      // +0x00
    value_type* Last;                                       // +0x04
    value_type* EndOfStorage;                               // +0x08
    C           Compare;                                    // +0x0c (empty, padded)
    const_iterator end() const { return Last; }
    const_iterator LowerBound(const K& key) const;          // 0x0069C740
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
    while (count > 0) {
        int half = count / 2;
        const value_type* middle = first + half;
        if (comp(*middle, key)) {
            first = middle + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return first;
}
#pragma auto_inline(on)

class CSoundMap {
public:
    CVectorMap<unsigned long, CSoundPair, CKeyPairCompareLess<unsigned long, CSoundPair> > SoundMap;      // +0x00
    CVectorMap<unsigned long, CDefString, CKeyPairCompareLess<unsigned long, CDefString> > CriteriaMap;   // +0x10
    const CDefString* GetPCriteriaString(const CCharString& name) const;
};

const CDefString* CSoundMap::GetPCriteriaString(const CCharString& name) const
{
    unsigned long crc = name.GetCRC();
    CVectorMap<unsigned long, CDefString, CKeyPairCompareLess<unsigned long, CDefString> >::const_iterator it =
        CriteriaMap.Find(crc);
    if (it != CriteriaMap.end())
        return &it->second;
    return 0;
}