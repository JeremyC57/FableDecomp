// CNodeChainOwner_00cb1210::DeleteNodeChain() -- frees every node of the singly linked chain at +8 (next at +4), reading each successor before its node is deleted
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB1210, length 28 bytes.
// Callees: operator delete @ 0x00BFE9BC (`add esp,4`: not a size TU). Shape: `victim = node; node = node->next; delete victim` (the successor is read from
// the old register before the delete; a trivially destructible node needs no null test). The chunk label (CEngineLandscapePatch::DestroyProceduralTextureVertexBuffers)
// is propagated from an unrelated body; the chain head is modelled locally. The head is not cleared.
struct SChainNode_00cb1210 {
    unsigned char       _pad[4];
    SChainNode_00cb1210* m_pNext;                  // +0x04
};

struct CNodeChainOwner_00cb1210 {
    unsigned char       _pad[8];
    SChainNode_00cb1210* m_pHead;                  // +0x08
    void DeleteNodeChain();
};

void CNodeChainOwner_00cb1210::DeleteNodeChain()
{
    SChainNode_00cb1210* node = m_pHead;
    while (node) {
        SChainNode_00cb1210* victim = node;
        node = node->m_pNext;
        delete victim;
    }
}
