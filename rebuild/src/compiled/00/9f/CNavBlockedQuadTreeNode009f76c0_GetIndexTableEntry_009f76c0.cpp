// CNavBlockedQuadTreeNode_009f76c0::GetIndexTableEntry() -- the address of the dword at +8 of the record this object points to (+0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F76C0, length 32 bytes.
// Leaf: GetIndexTableEntry
// The chunk label (CNavBlockedQuadTreeNode::AddToIndexTable) is propagated; named by address. Shape: an UNOPTIMISED translation unit (`this` spilled and reloaded), `ret`.
#pragma optimize("", off)
struct SIndexRecord_009f76c0 {
    long m_0;
    long m_4;
    long m_entry;                                  // +0x08
};

struct CNavBlockedQuadTreeNode_009f76c0 {
    SIndexRecord_009f76c0* m_pRecord;              // +0x00
    long* GetIndexTableEntry();
};

long* CNavBlockedQuadTreeNode_009f76c0::GetIndexTableEntry()
{
    return &m_pRecord->m_entry;
}