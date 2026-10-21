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