// CTools::GetWorld(const CWorldOwnerRef&) -- follows a fixed chain of owner pointers from the given reference to the world/handle table held at +0x38 of the final record
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C62150, length 20 bytes (`ret 4`, ecx unused).
// Callers: 0x00C62190 (the landed CHandleOwner_00c62180::ReleaseHandle declares it `__stdcall` and calls
// its slot 22 on the result), 0x00C6229C, 0x00C622E7, 0x00C622FC.
// Shape: a static `__stdcall` member (no `this`, callee pops the one argument) taking a reference; each
// hop is one load: [ref] -> +4 -> +0 -> +4 -> +0x38. The intermediate records are opaque here and named
// by position only.
struct CWorld;

struct CWorldRecord {                              // the record that owns the world pointer
    unsigned char _pad[0x38];
    CWorld*       m_pWorld;                        // +0x38
};
struct CWorldLink {
    void*         m_first;                         // +0x00
    CWorldRecord* m_pRecord;                       // +0x04
};
struct CWorldLinkHolder {
    CWorldLink*   m_pLink;                         // +0x00
};
struct CWorldOwner {
    void*             m_first;                     // +0x00
    CWorldLinkHolder* m_pHolder;                   // +0x04
};
struct CWorldOwnerRef {
    CWorldOwner*  m_pOwner;                        // +0x00
};

struct CTools {
    static CWorld* __stdcall GetWorld(const CWorldOwnerRef& ref);
};

CWorld* __stdcall CTools::GetWorld(const CWorldOwnerRef& ref)
{
    return ref.m_pOwner->m_pHolder->m_pLink->m_pRecord->m_pWorld;
}