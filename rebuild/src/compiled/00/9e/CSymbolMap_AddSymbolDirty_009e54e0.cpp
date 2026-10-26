// CSymbolMap::AddSymbolDirty(const CCharString& group, const CCharString& name, long val) -- record the name (when duplicate checking is on) and append the symbol under the given group to the unsorted map
// Leaf: AddSymbolDirty
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E54E0, 182 bytes, `ret 0Ch`.
// Identity: Ego_r.pdb `?AddSymbolDirty@CSymbolMap@@QAEXABVCCharString@@0J@Z` (the one-string overload is 0x009E5420).
// Callees: std::set<CCharString>::insert @ 0x004AC380 (pair<iterator,bool> result dropped), CCharString copy ctor
//   @ 0x0099EC30, std::vector<pair<CCharString,CSymbolInfo> >::push_back @ 0x009E7AB0, ~CCharString @ 0x0099EAE0.
// Layout (Ego_r.pdb CSymbolMap, retail-sized): SLONGMap +0x04 (CVectorMap: 12-byte vector, Compare +0x0c, Dirty +0x0d),
//   NamesUsed +0x14 (12-byte set), CheckForDuplicateSymbols +0x20.
// Shape (the module needs `__forceinline` on Add, the pair ctor and CSymbolInfo's memberwise copy): CVectorMap::Add (landed out of line at 0x009E7C00 for another caller) is inlined: the pair temporary is built from
//   the key and the CSymbolInfo temporary, pushed, destroyed, then Dirty is set before the info temporary dies; the
//   insert result lives in the `if` scope and shares the info temporary's frame slot.
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
};
struct SSetIterator { void* m_pNode; };
struct SSetInsertResult {                          // pair<iterator, bool>
    SSetIterator first;
    bool         second;
    SSetInsertResult() { }
};
struct CCharStringSet {                            // std::set<CCharString> (12 bytes in retail)
    unsigned char m_comp[4];
    void*         m_pHead;
    unsigned long m_size;
    SSetInsertResult insert(const CCharString& value);        // 0x004AC380
};
struct CSymbolInfo {                               // CSymbolMap::CSymbolInfo (12 bytes)
    CCharString GroupName;                         // +0x00
    CCharString EntryName;                         // +0x04
    long        Val;                               // +0x08
    __forceinline CSymbolInfo(const CCharString& group, const CCharString& entry, long val) : GroupName(group), EntryName(entry), Val(val) {}
    __forceinline CSymbolInfo(const CSymbolInfo& other) : GroupName(other.GroupName), EntryName(other.EntryName), Val(other.Val) {}
};
template <class T1, class T2> struct pair {        // STLport pair
    T1 first;
    T2 second;
    __forceinline pair(const T1& a, const T2& b) : first(a), second(b) {}
};
template <class T> struct vector {                 // retail STLport vector: 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void push_back(const T& value);                // 0x009E7AB0 for pair<CCharString,CSymbolInfo>
};
struct CCompareString {
};
template <class K, class V, class C> class CVectorMap { public:
    vector< pair<K, V> > Data;                     // +0x00
    C                    Compare;                  // +0x0c
    bool                 Dirty;                    // +0x0d
    __forceinline void Add(const K& key, const V& value) { Data.push_back(pair<K, V>(key, value)); Dirty = true; }
};
class CSymbolMap { public:
    long                                                    _padding_;               // +0x00
    CVectorMap<CCharString, CSymbolInfo, CCompareString>    SLONGMap;                // +0x04
    CCharStringSet                                          NamesUsed;               // +0x14
    bool                                                    CheckForDuplicateSymbols; // +0x20
    void AddSymbolDirty(const CCharString& group, const CCharString& name, long val);
};
void CSymbolMap::AddSymbolDirty(const CCharString& group, const CCharString& name, long val)
{
    if (CheckForDuplicateSymbols)
        NamesUsed.insert(name);
    SLONGMap.Add(name, CSymbolInfo(group, name, val));
}