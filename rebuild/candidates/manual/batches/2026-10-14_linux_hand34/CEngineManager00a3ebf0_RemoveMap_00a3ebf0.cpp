// CEngineManager_00a3ebf0::RemoveMap(void* map) -- tell every registered listener (the sentinel list at +0x4c) through virtual slot 9, then the owner object at +0x44 through its slot 9
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3EBF0, length 49 bytes.
// Callees: virtual slot 9 (+0x24) on each listener (the object at node+8) and on the object at +0x44. Shape: the argument is loaded into ebx before the
// loop; the node runs in esi, the listener straight into ecx; the loop test re-reads the sentinel (`cmp esi,[edi+4Ch]`); the final call reuses ebx.
// The chunk label "CEngineManager::RemoveMap" is propagated (the CEngineManager list sits at +4); the class is named by address.
struct CMapListener_00a3ebf0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void RemoveMap(void* map);             // slot 9 (+0x24)
};

struct SListenerNode {
    SListenerNode*         m_pNext;
    SListenerNode*         m_pPrev;
    CMapListener_00a3ebf0* m_pListener;            // +0x08
};

struct CEngineManager_00a3ebf0 {
    unsigned char          _pad0[0x44];
    CMapListener_00a3ebf0* m_pOwner;               // +0x44
    unsigned char          _pad1[4];
    SListenerNode*         m_pListeners;           // +0x4c (sentinel node)
    void RemoveMap(void* map);
};

void CEngineManager_00a3ebf0::RemoveMap(void* map)
{
    for (SListenerNode* node = m_pListeners->m_pNext; node != m_pListeners; node = node->m_pNext)
        node->m_pListener->RemoveMap(map);
    m_pOwner->RemoveMap(map);
}
