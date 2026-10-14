// CEngineTreeFilerBase::DeleteTree(STreeNode* node) -- delete every child (the chain from +0x10 through +0xc links, each recursively), then free the node
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF29E0, length 46 bytes.
// Callees: itself @ 0x00BF29E0 (on `this`, kept in edi), operator delete @ 0x00BFE9BC (the node is a POD: no destructor call). Shape: the next sibling is read before
// the recursive call (`mov esi,[eax+0Ch]`), the loop test is `test esi,esi ; mov eax,esi ; jne`; `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct STreeNode_00bf29e0 {
    unsigned char      _pad[0xc];
    STreeNode_00bf29e0* m_pNextSibling;            // +0x0c
    STreeNode_00bf29e0* m_pFirstChild;             // +0x10
};

struct CEngineTreeFilerBase {
    void DeleteTree(STreeNode_00bf29e0* node);
};

void CEngineTreeFilerBase::DeleteTree(STreeNode_00bf29e0* node)
{
    STreeNode_00bf29e0* child = node->m_pFirstChild;
    while (child) {
        STreeNode_00bf29e0* next = child->m_pNextSibling;
        DeleteTree(child);
        child = next;
    }
    delete node;
}