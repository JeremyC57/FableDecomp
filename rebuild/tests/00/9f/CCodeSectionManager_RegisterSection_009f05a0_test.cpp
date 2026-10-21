// Behaviour test for 009f05a0: src/009f05a0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CCodeSectionManager::RegisterSection(CCharString shortName, CCharString longName, bool load, bool loaded)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCodeSectionManager::RegisterSection(CCharString shortName, CCharString longName, bool load, bool loaded) -- allocate the next section handle; an already-registered short name returns its existing handle, otherwise both names are mapped to the new handle, its CSectionDesc (names, Loaded, Size 0) is stored, and the section is loaded or unloaded when `load` and `loaded` disagree; returns the handle
// Leaf: ?RegisterSection@CCodeSectionManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F05A0, length 300 bytes (`ret 10h`; the callee destroys the by-value strings).
// Callees: map<CCharString,unsigned long>::find @ 0x009D2DD0 / operator[] @ 0x0042D131, CCharString() 0x0099E4B0, operator=
//          0x0099EFB0, ~CCharString 0x0099EAE0, map<unsigned long,CSectionDesc>::operator[] @ 0x009F1470 (manifest label
//          "CCodeSectionManager_RegisterSection" is the callee's), LoadSection 0x009F02C0, UnloadSection 0x009F02F0; data:
//          the handle counter 0x01396858.
// Layout: Ego_r.pdb CCodeSectionManager with 12-byte retail maps: ShortNameToHandle +0x04, LongNameToHandle +0x10,
//          Sections +0x1c, SectionLoadingEnabled +0x28; CSectionDesc {ShortName, LongName, Loaded, Size} (16 bytes).
// Shape: the handle is `++counter` held in ebp; the Sections key is a separate frame word read back from the counter
//          (`sectionKey = g_nextCodeSectionHandle`, stored at entry), because the map's operator[] takes it by const reference.
//          Copying the handle into the key swaps `this`/handle between edi and ebp. A single address-taken handle is
//          re-read after every call (315 bytes).
struct CCharString {
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};

template <class K, class V> struct CRetailMap {        // STLport map: header node, count, compare (12 bytes)
    struct Node { void* _M_links[4]; K first; V second; };
    Node*         _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
    Node* find(const K& key);                          // out of line
    Node* end() const { return _M_header; }
    V& operator[](const K& key);                       // out of line
};

class CCodeSectionManager {
public:
    struct CSectionDesc {
        CCharString   ShortName;                       // +0x00
        CCharString   LongName;                        // +0x04
        bool          Loaded;                          // +0x08
        long          Size;                            // +0x0c
    };
    virtual void v00();
    CRetailMap<CCharString, unsigned long> ShortNameToHandle;   // +0x04 (find 0x009D2DD0, [] 0x0042D131)
    CRetailMap<CCharString, unsigned long> LongNameToHandle;    // +0x10
    CRetailMap<unsigned long, CSectionDesc> Sections;           // +0x1c ([] 0x009F1470)
    bool SectionLoadingEnabled;                                 // +0x28
    void LoadSection(unsigned long handle);                     // 0x009F02C0
    void UnloadSection(unsigned long handle);                   // 0x009F02F0
    unsigned long RegisterSection(CCharString shortName, CCharString longName, bool load, bool loaded);
};

extern unsigned long g_nextCodeSectionHandle;          // 0x01396858

unsigned long CCodeSectionManager::RegisterSection(CCharString shortName, CCharString longName, bool load, bool loaded)
{
    unsigned long handle = ++g_nextCodeSectionHandle;
    unsigned long sectionKey = g_nextCodeSectionHandle;
    CRetailMap<CCharString, unsigned long>::Node* it = ShortNameToHandle.find(shortName);
    if (it != ShortNameToHandle.end())
        return it->second;
    ShortNameToHandle[shortName] = handle;
    LongNameToHandle[longName] = handle;
    CSectionDesc desc;
    desc.Loaded = loaded;
    desc.LongName = longName;
    desc.ShortName = shortName;
    desc.Size = 0;
    Sections[sectionKey] = desc;
    if (load) {
        if (!loaded)
            LoadSection(handle);
    } else if (loaded)
        UnloadSection(handle);
    return handle;
}

// ---- stubs and driver ----
unsigned long g_nextCodeSectionHandle = 0;
static int g_strCtors = 0, g_strDtors = 0, g_assigns = 0, g_loads = 0, g_unloads = 0;
static unsigned long g_loadHandle = 0, g_unloadHandle = 0;
CCharString::CCharString() { PStringData = 0; ++g_strCtors; }
CCharString::CCharString(const CCharString& other) { PStringData = other.PStringData; ++g_strCtors; }
CCharString::~CCharString() { ++g_strDtors; }
CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; ++g_assigns; return *this; }
// tiny fixed-capacity maps keyed by the string's data pointer / the handle
static CRetailMap<CCharString, unsigned long>::Node g_strNodes[2][8]; static int g_strCount[2];
static CRetailMap<unsigned long, CCodeSectionManager::CSectionDesc>::Node g_secNodes[8]; static int g_secCount;
static CRetailMap<CCharString, unsigned long>::Node g_endNode;
static int MapIndexOf(void* map, CCodeSectionManager* m) { return map == &m->ShortNameToHandle ? 0 : 1; }
static CCodeSectionManager* g_mgr;
template <> CRetailMap<CCharString, unsigned long>::Node* CRetailMap<CCharString, unsigned long>::find(const CCharString& key)
{
    int k = MapIndexOf(this, g_mgr);
    for (int i = 0; i < g_strCount[k]; ++i) if (g_strNodes[k][i].first.PStringData == key.PStringData) return &g_strNodes[k][i];
    return _M_header;
}
template <> unsigned long& CRetailMap<CCharString, unsigned long>::operator[](const CCharString& key)
{
    int k = MapIndexOf(this, g_mgr);
    for (int i = 0; i < g_strCount[k]; ++i) if (g_strNodes[k][i].first.PStringData == key.PStringData) return g_strNodes[k][i].second;
    g_strNodes[k][g_strCount[k]].first.PStringData = key.PStringData;
    return g_strNodes[k][g_strCount[k]++].second;
}
template <> CCodeSectionManager::CSectionDesc& CRetailMap<unsigned long, CCodeSectionManager::CSectionDesc>::operator[](const unsigned long& key)
{
    for (int i = 0; i < g_secCount; ++i) if (g_secNodes[i].first == key) return g_secNodes[i].second;
    g_secNodes[g_secCount].first = key;
    return g_secNodes[g_secCount++].second;
}
void CCodeSectionManager::v00() { }
void CCodeSectionManager::LoadSection(unsigned long handle) { ++g_loads; g_loadHandle = handle; }
void CCodeSectionManager::UnloadSection(unsigned long handle) { ++g_unloads; g_unloadHandle = handle; }
static CCharString Str(const char* p) { CCharString s; s.PStringData = (void*)p; return s; }
int main()
{
    int ctors0 = g_strCtors, dtors0 = g_strDtors;   // the static node arrays construct strings before main
    static unsigned char raw[sizeof(CCodeSectionManager)];
    CCodeSectionManager* m = (CCodeSectionManager*)raw; g_mgr = m;
    m->ShortNameToHandle._M_header = &g_endNode;
    g_nextCodeSectionHandle = 41;
    static const char a[] = "A", al[] = "Alpha", b[] = "B", bl[] = "Beta", c[] = "C", cl[] = "Gamma";
    unsigned long h = m->RegisterSection(Str(a), Str(al), true, false);       // load requested, not loaded: LoadSection
    CHECK(h == 42 && g_nextCodeSectionHandle == 42 && g_loads == 1 && g_loadHandle == 42 && g_unloads == 0);
    CHECK(g_strCount[0] == 1 && g_strNodes[0][0].second == 42 && g_strCount[1] == 1 && g_strNodes[1][0].second == 42);
    CHECK(g_secCount == 1 && g_secNodes[0].first == 42 && g_secNodes[0].second.ShortName.PStringData == a);
    CHECK(g_secNodes[0].second.LongName.PStringData == al && !g_secNodes[0].second.Loaded && g_secNodes[0].second.Size == 0);
    h = m->RegisterSection(Str(b), Str(bl), false, true);                      // loaded but not wanted: UnloadSection
    CHECK(h == 43 && g_unloads == 1 && g_unloadHandle == 43 && g_loads == 1 && g_secNodes[1].second.Loaded);
    h = m->RegisterSection(Str(c), Str(cl), true, true);                       // agree: neither
    CHECK(h == 44 && g_loads == 1 && g_unloads == 1 && g_secCount == 3);
    h = m->RegisterSection(Str(a), Str(cl), true, false);                      // already registered: existing handle, counter still advances
    CHECK(h == 42 && g_nextCodeSectionHandle == 45 && g_secCount == 3 && g_loads == 1);
    CHECK(g_strCtors - ctors0 == g_strDtors - dtors0);
    if (fails == 0) printf("009f05a0_TEST PASS\n");
    return fails;
}
