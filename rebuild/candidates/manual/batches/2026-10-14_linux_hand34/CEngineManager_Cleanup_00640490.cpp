// CEngineManager::Cleanup() -- walk the registered-component list (the sentinel at +4) doing nothing per node: the per-component cleanup inlined to an empty body
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00640490, length 23 bytes.
// Callees: none. Shape: `mov ecx,[ecx+4] ; mov eax,[ecx] ; cmp eax,ecx ; je ; loop: mov eax,[eax] ; cmp eax,ecx ; jne loop ; ret` -- the loop survives because the
// node pointer chase is a memory read the compiler keeps; the loop head is aligned with `lea esp,[esp]`. Modelled as a loop over the list calling an empty
// inline per component.
struct SComponentNode {
    SComponentNode* m_pNext;
    SComponentNode* m_pPrev;
    void*           m_pComponent;                  // +0x08
};

struct CEngineManager {
    unsigned char   _pad[4];
    SComponentNode* m_pComponents;                 // +0x04 (sentinel node)
    void CleanupComponent(SComponentNode* node) { }   // nothing left to do per component in the retail build
    void Cleanup();
};

void CEngineManager::Cleanup()
{
    for (SComponentNode* node = m_pComponents->m_pNext; node != m_pComponents; node = node->m_pNext)
        CleanupComponent(node);
}
