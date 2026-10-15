// CThingComponentSet::Add(const CCharString& name, bool flag) -- look the component name up in the registered-TC table and, when found, append an entry {def-string name, registered type, flag} to the small vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00870750, length 68 bytes.
// Leaf: ?Add@CThingComponentSet
// Callees: the registered-TC lookup @ 0x00686F60 (a `__fastcall` static taking the name in ecx: lower bound in the global
// vector map @ 0x013BA89C, string compare, `&entry.value` or null; the manifest's "CVectorMap_FindMeshVolume" is
// propagated), CDefString::CDefString(const CCharString&) @ 0x009D5060 (table position from the global CDefStringTable
// @ 0x013CA828; the manifest's "CRegion::SetMiniMapRegionExitTextOffsetX" is propagated), CSmallVector<CEntry,8>::
// push_back(const CEntry&) @ 0x00870480 (9-byte records: grows the signed-char size at +4, copies dword/dword/byte).
// The chunk label "CTCActionUseScriptedHook::SetEntranceAsLocked" is propagated and rejected: Ego_r.pdb has
// `?Add@CThingComponentSet@@QAEXABVCCharString@@_N@Z` (two arguments, `ret 8`) and CThingComponentSet = {CSmallVector<CEntry,8>
// Entries} at +0; the registered info is {allocator +0, type +4, CCharString name +8} (`CRegisteredTCInfo(alloc, ulong, name)`).
// Shape: the entry is a packed 9-byte temporary (frame offset 3 of a 12-byte frame) built by an inline constructor: the
// type is loaded into esi before the CDefString call, the flag after it, both stored once the push_back argument is out.
struct CThing;
struct CTCBase;
struct CCharString { void* m_pData; };

struct CDefString {
    long TablePos;                             // +0
    CDefString(const CCharString& name);       // 0x009D5060
};

struct CRegisteredTCInfo {
    CTCBase* (__fastcall* m_pfnAllocate)(CThing& thing);   // +0x00
    unsigned long m_type;                      // +0x04
    CCharString   m_name;                      // +0x08
};

const CRegisteredTCInfo* __fastcall FindRegisteredTCInfo(const CCharString& name);   // 0x00686F60

#pragma pack(push, 1)
struct CEntry {                                // CThingComponentSet::CEntry, 9 bytes
    CDefString    Name;                        // +0
    unsigned long Type;                        // +4
    bool          Flag;                        // +8
    CEntry(const CCharString& name, unsigned long type, bool flag) : Name(name), Type(type), Flag(flag) { }
};
#pragma pack(pop)

struct CSmallVector_CEntry {                   // CSmallVector<CThingComponentSet::CEntry, 8>
    CEntry*     Start;                         // +0
    signed char Size;                          // +4
    void push_back(const CEntry& entry);       // 0x00870480
};

struct CThingComponentSet {
    CSmallVector_CEntry Entries;               // +0
    void Add(const CCharString& name, bool flag);
};

void CThingComponentSet::Add(const CCharString& name, bool flag)
{
    const CRegisteredTCInfo* info = FindRegisteredTCInfo(name);
    if (info)
        Entries.push_back(CEntry(info->m_name, info->m_type, flag));
}