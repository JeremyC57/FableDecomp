// Behaviour test for CSymbolMap::AddSymbolDirty (009e5420): optional NamesUsed insert, then append (name, {group, name, val}) and mark the map dirty.
#include <cstdio>
#include <cstring>
static int g_live = 0;
class CCharString { public:
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { ++g_live; }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_live; }
    ~CCharString() { --g_live; }
};
CCharString g_CCharString_013caa30("NULL");
struct SSetIterator { void* m_pNode; };
struct SSetInsertResult { SSetIterator first; bool second; SSetInsertResult() { } };
struct CCharStringSet {
    const char* m_last; int m_count;
    SSetInsertResult insert(const CCharString& value) { m_last = value.m_pData; ++m_count; SSetInsertResult r; r.second = true; return r; }
};
struct CSymbolInfo {
    CCharString GroupName; CCharString EntryName; long Val;
    CSymbolInfo(const CCharString& group, const CCharString& entry, long val) : GroupName(group), EntryName(entry), Val(val) {}
    CSymbolInfo(const CSymbolInfo& other) : GroupName(other.GroupName), EntryName(other.EntryName), Val(other.Val) {}
};
template <class T1, class T2> struct pair { T1 first; T2 second; pair(const T1& a, const T2& b) : first(a), second(b) {} };
struct SRecord { const char* key; const char* group; const char* entry; long val; int liveAtPush; };
static SRecord g_pushed[4]; static int g_pushes = 0; static bool* g_dirtyAtPush = 0; static bool g_dirtyBefore = true;
template <class T> struct vector {
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    void push_back(const T& v)
    {
        SRecord r = { v.first.m_pData, v.second.GroupName.m_pData, v.second.EntryName.m_pData, v.second.Val, g_live };
        g_pushed[g_pushes++] = r; if (g_dirtyAtPush) g_dirtyBefore = *g_dirtyAtPush;
    }
};
struct CCompareString { };
template <class K, class V, class C> class CVectorMap { public:
    vector< pair<K, V> > Data; C Compare; bool Dirty;
    void Add(const K& key, const V& value) { Data.push_back(pair<K, V>(key, value)); Dirty = true; }
};
class CSymbolMap { public:
    long _padding_; CVectorMap<CCharString, CSymbolInfo, CCompareString> SLONGMap; CCharStringSet NamesUsed; bool CheckForDuplicateSymbols;
    void AddSymbolDirty(const CCharString& name, long val);
};
void CSymbolMap::AddSymbolDirty(const CCharString& name, long val)
{
    if (CheckForDuplicateSymbols)
        NamesUsed.insert(name);
    SLONGMap.Add(name, CSymbolInfo(g_CCharString_013caa30, name, val));
}
int main()
{
    CCharString n("Sword"); CCharString g("Weapons"); (void)g;
    int base = g_live;
    CSymbolMap m; std::memset(&m, 0, sizeof(m)); g_dirtyAtPush = &m.SLONGMap.Dirty;
    m.CheckForDuplicateSymbols = false; m.AddSymbolDirty(n, 7);
    if (m.NamesUsed.m_count != 0 || g_pushes != 1 || !m.SLONGMap.Dirty || g_dirtyBefore || g_live != base) { std::printf("009e5420_TEST FAIL\n"); return 1; }
    const char* wantGroup = g_CCharString_013caa30.m_pData;
    if (g_pushed[0].key != n.m_pData || g_pushed[0].entry != n.m_pData || g_pushed[0].group != wantGroup || g_pushed[0].val != 7
        || g_pushed[0].liveAtPush != base + 5) { std::printf("009e5420_TEST FAIL\n"); return 1; }
    m.SLONGMap.Dirty = false; m.CheckForDuplicateSymbols = true; m.AddSymbolDirty(n, 7);
    if (m.NamesUsed.m_count != 1 || m.NamesUsed.m_last != n.m_pData || g_pushes != 2 || !m.SLONGMap.Dirty || g_live != base) { std::printf("009e5420_TEST FAIL\n"); return 1; }
    std::printf("009e5420_TEST PASS\n");
    return 0;
}