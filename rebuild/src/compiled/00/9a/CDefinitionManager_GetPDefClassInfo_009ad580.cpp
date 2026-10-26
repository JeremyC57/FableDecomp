// CDefinitionManager::GetPDefClassInfo(const CCharString& name) -- the def-class record registered under a class name (looked up by the name's CRC), or null
// Leaf: ?GetPDefClassInfo@CDefinitionManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AD580, 98 bytes (+ int3 padding), `ret 4`. Catalog name "CVectorMap_FindByKey";
// Ego_r PDB: `?GetPDefClassInfo@CDefinitionManager@@IAEPAVCDefClassInfo@1@ABVCCharString@@@Z` (protected; the
// unsigned-long overload `...@1@K@Z` is the landed 0x009AD350 "FindMapValueAt78", whose body this one inlines).
// Callees: CCRC::Calc(seed, data, byteCount) @ 0x004014A0 (`__fastcall`), CVectorMap<unsigned long, CDefClassInfo,
// CKeyPairCompareLess<...> >::LowerBound(const unsigned long&) @ 0x009B2230. Literal: the shared "" @ 0x0129AAF4.
// Layout: retail CDefinitionManager keeps DefClassInfo (CVectorMap: STLport vector {First, Last, EndOfStorage} of
// 24-byte pair<unsigned long, CDefClassInfo> + the comparator) at +0x78 (Ego_r +0x84: its vectors are 16 bytes).
// Shape: name.GetCRC() inline (GetLength then GetPString, right to left, each with its own null test) into a NAMED
// local, then the
// inlined GetPDefClassInfo(unsigned long key): the by-value key is homed in the dead `name` argument slot (its address
// goes to LowerBound), Find() = lower bound, end() when the key sorts before the entry; a miss returns null
// (`xor eax,eax` at the return), a hit the entry's value at +4.
class CCharStringData { public:
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
};
class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->PData; }
    unsigned long GetCRC() const;
};
struct CCRC {
    static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long byteCount);   // 0x004014A0
};
inline unsigned long CCharString::GetCRC() const
{
    return CCRC::Calc(0, GetPString(), GetLength());
}

template <class A, class B> struct pair {
    A first;                                    // +0x00
    B second;                                   // +0x04
};
template <class K, class V> class CKeyPairCompareLess { public:
};
template <class K, class V, class C> class CVectorMap {
public:
    typedef pair<K, V> value_type;
    value_type* First;                          // +0x00
    value_type* Last;                           // +0x04
    value_type* EndOfStorage;                   // +0x08
    C           Compare;                        // +0x0c
    value_type* LowerBound(const K& key);       // 0x009B2230 (same-TU body below)
    value_type* end() { return Last; }
    value_type* Find(const K& key)
    {
        value_type* it = LowerBound(key);
        if (it == end() || key < it->first)
            it = end();
        return it;
    }
};

// The out-of-line LowerBound @ 0x009B2230 is compiled in the same translation unit (the landed body): its visible
// body tells VC7.1 that the call only reads the key, so the CRC stays live in edi for the compare after the call.
#pragma auto_inline(off)
template <class K, class V, class C>
__declspec(noinline) typename CVectorMap<K, V, C>::value_type* CVectorMap<K, V, C>::LowerBound(const K& key)
{
    value_type* first = First;
    int count = Last - First;
    while (count > 0) {
        int half = count / 2;
        value_type* middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}
#pragma auto_inline(on)

class CDefinitionManager {
public:
    class CDefClassInfo {                       // 20 bytes
    public:
        long Data[5];
    };
    unsigned char _pad_0x00[0x78];
    CVectorMap<unsigned long, CDefClassInfo, CKeyPairCompareLess<unsigned long, CDefClassInfo> > DefClassInfo;   // +0x78
protected:
    CDefClassInfo* GetPDefClassInfo(unsigned long key)   // 0x009AD350 (inlined here)
    {
        pair<unsigned long, CDefClassInfo>* it = DefClassInfo.Find(key);
        if (it == DefClassInfo.end())
            return 0;
        return &it->second;
    }
    CDefClassInfo* GetPDefClassInfo(const CCharString& name);
};

CDefinitionManager::CDefClassInfo* CDefinitionManager::GetPDefClassInfo(const CCharString& name)
{
    unsigned long crc = name.GetCRC();
    return GetPDefClassInfo(crc);
}