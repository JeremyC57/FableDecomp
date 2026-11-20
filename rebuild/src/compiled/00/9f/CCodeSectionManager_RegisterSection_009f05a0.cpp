// CCodeSectionManager::RegisterSection(CCharString shortName, CCharString longName, bool load, bool loaded) -- allocate the next section handle; an already-registered short name returns its existing handle, otherwise both names are mapped to the new handle, its CSectionDesc (names, Loaded, Size 0) is stored, and the section is loaded or unloaded when `load` and `loaded` disagree; returns the handle
// Leaf: ?RegisterSection@CCodeSectionManager@@QAEKVCCharString@@0_N1@Z
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
// Naming (NF180, 2026-11-20, q4394): the local CCharString / CRetailMap stand-ins replaced by fable_string.h and the shared STLport
//   map (stlport/fable_map.h: _M_find out of line, FABLE_RETAIL_STLP_TREE_MFIND_DECL; operator[] out of line,
//   FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY), so the lookup binds the _M_find spelling the self row 0x009D2DD0 defines and the
//   map<CCharString, unsigned long>::operator[] call the STLport spelling the landed 0x009CE050 binds at 0x0042D131. CCodeSectionManager
//   has no shared header: the row-local model keeps its token (and its nested CSectionDesc), its maps now typed std::map (Ego_r.pdb
//   ShortNameToHandle / LongNameToHandle / SectionDesc).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#include "fable_string.h"
#include "stlport/fable_map.h"

class CCodeSectionManager {
public:
    struct CSectionDesc {
        CCharString   ShortName;                       // +0x00
        CCharString   LongName;                        // +0x04
        bool          Loaded;                          // +0x08
        long          Size;                            // +0x0c
    };
    virtual void v00();
    std::map<CCharString, unsigned long> ShortNameToHandle;     // +0x04 (_M_find 0x009D2DD0, [] 0x0042D131)
    std::map<CCharString, unsigned long> LongNameToHandle;      // +0x10
    std::map<unsigned long, CSectionDesc> Sections;             // +0x1c ([] 0x009F1470)
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
    std::map<CCharString, unsigned long>::iterator it = ShortNameToHandle.find(shortName);
    if (it != ShortNameToHandle.end())
        return (*it).second;
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