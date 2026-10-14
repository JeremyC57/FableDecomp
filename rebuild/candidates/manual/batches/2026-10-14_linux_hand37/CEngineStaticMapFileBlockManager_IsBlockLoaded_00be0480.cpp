// CEngineStaticMapFileBlockManager::IsBlockLoaded(long block) -- find the block's node in the set at +8; false when it is the end sentinel, else whether its async data has finished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE0480, length 52 bytes.
// Callees: the set's find @ 0x00BE0930 (on +8: the node result by reference in the one-word frame, the key by reference), CBankFileAsyncData::IsFinished @ 0x009D54E0
// (on the data at +0x1c of the node; a bool in al). Shape: the set address is formed with `lea esi,[ecx+8]` and its first word re-read for the sentinel compare;
// `xor al,al` for the miss.
struct CBankFileAsyncData {
    bool IsFinished();                             // 0x009D54E0
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
    bool IsBlockLoaded(long block);
};

bool CEngineStaticMapFileBlockManager::IsBlockLoaded(long block)
{
    SBlockNode_00be0400* node;
    m_blocks.Find(node, block);
    if (node == m_blocks.m_pEnd)
        return false;
    return node->m_pData->IsFinished();
}
