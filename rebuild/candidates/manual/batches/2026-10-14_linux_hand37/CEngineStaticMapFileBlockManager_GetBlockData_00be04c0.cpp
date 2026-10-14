// CEngineStaticMapFileBlockManager::GetBlockData(long block) -- find the block's node in the set at +8 and return its async data's buffer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE04C0, length 32 bytes.
// Callees: the set's find @ 0x00BE0930 (on +8: the node result by reference in the one-word frame, the key by reference), CBankFileAsyncData::GetData @ 0x009D5580 (on
// the data at +0x1c of the node). Shape: `push ecx` frame, `add ecx,8`, the result read back for the tail call's `this`.
struct CBankFileAsyncData {
    void* GetData();                               // 0x009D5580
};
struct SBlockNode_00be0400 {                        // the set node: the async data at +0x1c, the high-priority flag at +0x18, the pending count at +0x14
    unsigned char       _pad[0x14];
    long                m_pending;                 // +0x14
    bool                m_highPriority;            // +0x18
    unsigned char       _pad19[3];
    CBankFileAsyncData* m_pData;                   // +0x1c
};

struct CRetailBlockSet {                           // the retail set at +8: its first word is the end sentinel
    SBlockNode_00be0400* m_pEnd;
    void FindInsertionPoint(SBlockNode_00be0400*& result, const long& key);   // 0x00BE08E0
    void Find(SBlockNode_00be0400*& result, const long& key);                 // 0x00BE0930
};

struct CEngineStaticMapFileBlockManager {
    unsigned char   _pad[8];
    CRetailBlockSet m_blocks;                      // +0x08
    void* GetBlockData(long block);
};

void* CEngineStaticMapFileBlockManager::GetBlockData(long block)
{
    SBlockNode_00be0400* node;
    m_blocks.Find(node, block);
    return node->m_pData->GetData();
}
