// Grade: DIFFER(348v322) 80 diff lines (parked 264v322, 101 lines): retail's three owners of the read's count block are modelled -- `data` default-built then assigned from the returned temporary (add-ref, temp released), a `request` copy, and the record copy-constructed from it, released record/request/data after the insert; left: retail keeps the zero in no register (test reg,reg) where VC7.1 here hoists it into edi, the read's offset argument stays in edi, and the counted-pointer dtor zeroing / frame layout (0x24 v 0x20)
// CEngineStaticMapFileBlockManager::AddRequest(unsigned long offset, unsigned long size, bool highPriority) -- request a static-map file block: a new block starts an asynchronous read (high priority or the default read priority) and is inserted with one pending request; an existing block gets its pending count bumped and, for a normal request on a high-priority block, its read priority lowered
// Leaf: AddRequest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE06B0, length 322 bytes, `ret 0Ch`.
// Callees: the block set's FindInsertionPoint @ 0x00BE08E0 and insert @ 0x00BE0BF0, CBankFileAsyncEntry::
//   BeginReadDataAutoAlloc(ulong, ulong, EThreadedFilePriority) @ 0x009D6100, CBankFileAsyncData::SetPriority @ 0x009D5630,
//   operator delete @ 0x00BFE9BC; the default read priority global @ 0x0139A728.
// Layout: as the landed ReleaseRequest 0x00BE0400 (entry +0, block set +8; node pending +0x14, high flag +0x18, data
//   +0x1c) and the CCountedPointer {Object, count block} model of the landed BeginLoading 0x00AF1360.
void __fastcall DestroyCountedObject(void* object);
struct SCountBlock {
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
#include "engine/CCountedPointer.h"

enum EThreadedFilePriority { READ_PRIORITY_HIGH = 2 };
extern EThreadedFilePriority g_DefaultReadPriority;   // 0x0139A728
struct CBankFileAsyncData {
    void SetPriority(EThreadedFilePriority priority);   // 0x009D5630
};
struct CBankFileAsyncEntry {
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long offset, unsigned long size, EThreadedFilePriority priority);   // 0x009D6100
};
struct SBlockRecord {
    unsigned long                       Offset;
    long                                Pending;
    bool                                HighPriority;
    CCountedPointer<CBankFileAsyncData> Data;
    SBlockRecord(unsigned long offset, long pending, bool highPriority, const CCountedPointer<CBankFileAsyncData>& data)
        : Offset(offset), Pending(pending), HighPriority(highPriority), Data(data) { }
};
struct SBlockNode {
    unsigned char       _pad[0x14];
    long                m_pending;                 // +0x14
    bool                m_highPriority;            // +0x18
    unsigned char       _pad19[3];
    CBankFileAsyncData* m_pData;                   // +0x1c
};
struct CRetailBlockSet {
    SBlockNode* m_pEnd;
    void FindInsertionPoint(SBlockNode*& result, const unsigned long& key);   // 0x00BE08E0
    void insert(SBlockNode*& result, const SBlockRecord& record);             // 0x00BE0BF0
};
struct CEngineStaticMapFileBlockManager {
    CBankFileAsyncEntry* Entry;                    // +0x00
    unsigned long        _pad04;
    CRetailBlockSet      m_blocks;                 // +0x08
    void AddRequest(unsigned long offset, unsigned long size, bool highPriority);
};
void CEngineStaticMapFileBlockManager::AddRequest(unsigned long offset, unsigned long size, bool highPriority)
{
    SBlockNode* node;
    m_blocks.FindInsertionPoint(node, offset);
    if (node == m_blocks.m_pEnd) {
        CCountedPointer<CBankFileAsyncData> data;
        data = Entry->BeginReadDataAutoAlloc(offset, size, highPriority ? READ_PRIORITY_HIGH : g_DefaultReadPriority);
        CCountedPointer<CBankFileAsyncData> request(data);
        SBlockRecord record(offset, 1, highPriority, request);
        SBlockNode* inserted;
        m_blocks.insert(inserted, record);
        return;
    }
    if (!highPriority && node->m_highPriority)
        node->m_pData->SetPriority(g_DefaultReadPriority);
    ++node->m_pending;
}
