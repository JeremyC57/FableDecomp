// CCodeSectionManager::GetHandleFromLongName(CCharString longName) -- the section handle registered under this long name, or 0xFFFFFFFF when none is
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F0500, 50 bytes (`ret 4`; the callee destroys the by-value string).
// Label: the manifest's "NQuestManager::GetAnimationEntry" is propagated. Callers: LoadSectionLongName 0x004AF000 and
// UnloadSectionLongName 0x004AF030 (`LoadSection(GetHandleFromLongName(name))`) and two quest-manager sites (0x004B3CA3,
// 0x004B3DFC); FableWin.pdb lists ?GetHandleFromLongName@CCodeSectionManager@@QAEKVCCharString@@@Z.
// Callees: map<CCharString, unsigned long>::find @ 0x009D2DD0 (returns the node or the header), ~CCharString @ 0x0099EAE0.
// Layout: as the landed CCodeSectionManager::RegisterSection 0x009F05A0 (12-byte retail STLport maps): LongNameToHandle +0x10;
// the node's mapped handle at +0x14 (links +0..+0xc, key +0x10).
// Shape: the header is read AFTER the find (`cmp eax,[esi]`); the by-value parameter's destructor is inlined into both return
// paths (the miss path returns `or eax,-1`, the hit path parks the handle in esi across the destructor).
class CCharString { public:
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};

template <class K, class V> struct CRetailMap {        // STLport map: header node, count, compare (12 bytes)
    struct Node { void* _M_links[4]; K first; V second; };
    Node*         _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
    Node* find(const K& key);                          // out of line
    Node* end() const { return _M_header; }
};

class CCodeSectionManager {
public:
    virtual void v00();
    CRetailMap<CCharString, unsigned long> ShortNameToHandle;   // +0x04
    CRetailMap<CCharString, unsigned long> LongNameToHandle;    // +0x10 (find 0x009D2DD0)
    unsigned long GetHandleFromLongName(CCharString longName);
};

unsigned long CCodeSectionManager::GetHandleFromLongName(CCharString longName)
{
    CRetailMap<CCharString, unsigned long>::Node* it = LongNameToHandle.find(longName);
    if (it == LongNameToHandle.end())
        return 0xFFFFFFFF;
    return it->second;
}