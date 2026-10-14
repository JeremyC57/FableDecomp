// CEngineScene::ClearNonPersistent() -- walk the non-persistent chain at +0x24: for each node remember its +0x38 link, clear its four links (+0x38..+0x44), drop its reference count (+4) and destroy it through virtual slot 1 at zero; then clear the chain head
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B91980, length 58 bytes.
// Callees: virtual slot 1 (+4) of a node whose count reaches zero. Shape: the zero register edi serves the four link stores, the head test and the final store; the
// decremented count is stored before the `jne` (`dec eax ; ... ; mov [ecx+4],eax ; jne`); the loop head is aligned with `mov edi,edi`.
struct CSceneNode_00b91980 {
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+0x04)
    long                 m_references;             // +0x04
    unsigned char        _pad[0x30];
    CSceneNode_00b91980* m_pNext;                  // +0x38
    CSceneNode_00b91980* m_pPrev;                  // +0x3c
    CSceneNode_00b91980* m_pNextSkipped;           // +0x40
    CSceneNode_00b91980* m_pPrevSkipped;           // +0x44
};

struct CEngineScene {
    unsigned char        _pad[0x24];
    CSceneNode_00b91980* m_pNonPersistent;         // +0x24
    void ClearNonPersistent();
};

void CEngineScene::ClearNonPersistent()
{
    CSceneNode_00b91980* node = m_pNonPersistent;
    while (node) {
        CSceneNode_00b91980* next = node->m_pNext;
        node->m_pNext = 0;
        node->m_pPrev = 0;
        node->m_pNextSkipped = 0;
        node->m_pPrevSkipped = 0;
        if (--node->m_references == 0)
            node->Destroy();
        node = next;
    }
    m_pNonPersistent = 0;
}
