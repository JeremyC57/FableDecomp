// CVectorMap<CCharString,CSymbolInfo,CCompareString>::Add(const CCharString& key, const CSymbolInfo& value) -- appends the (key, value) pair to the unsorted record vector and marks the map dirty (sorted again on the next lookup)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E7C00, length 104 bytes (padded to 112).
// Label correction: the catalog row "CGameScriptInterface::AddGossipFactionToCategory" is propagated and wrong -- `this`
// holds the record vector at +0 (handed straight to the 16-byte-record push_back) and a bool at +0x0d, which is the
// retail layout of CVectorMap<CCharString,CSymbolMap::CSymbolInfo,CCompareString<CSymbolMap::CSymbolInfo>> (PDB:
// 16-byte vector, Compare +0x10, Dirty +0x11; retail vectors are 12 bytes, so Compare +0x0c, Dirty +0x0d). The
// neighbouring functions (0x009E7590 insertion sort, 0x009E7E40 introsort, 0x009E7C70 DeallocateUnusedMemory) are the
// same map's helpers.
// Callees: CCharString copy constructor @ 0x0099EC30 (x3), std::vector<std::pair<CCharString,CSymbolInfo> >::push_back @ 0x009E7AB0,
//          CCharString::~CCharString @ 0x0099EAE0 (x3).
// Shape: the pair temporary is built in a 16-byte frame (key, the info's two strings, the info's long) by the STLport
// pair constructor and the implicit memberwise CSymbolInfo copy; its three strings are destroyed in reverse order; no EH
// frame (module built without /EHsc).
// Leaf: ?Add@?$CVectorMap@
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
};
struct CSymbolInfo {                               // CSymbolMap::CSymbolInfo (12 bytes)
    CCharString m_name;                            // +0x00
    CCharString m_type;                            // +0x04
    long        m_value;                           // +0x08
};
template <class T1, class T2> struct pair {        // STLport pair
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
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
    void Add(const K& key, const V& value);
};

template <class K, class V, class C> void CVectorMap<K, V, C>::Add(const K& key, const V& value)
{
    Data.push_back(pair<K, V>(key, value));
    Dirty = true;
}

template class CVectorMap<CCharString, CSymbolInfo, CCompareString>;