// CRetailTree_004ae7b0::`scalar deleting destructor'(unsigned int flags) -- destroy a retail (VC6-era Dinkumware) tree: when it holds nodes (+4) erase the subtree under the head's parent (0x004AE490, the landed recursive free), relink the head (+8/+0xc to itself, +4 null) and zero the count; free the head node (0x00BFEA14) and the tree for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AE7B0, length 96 bytes.
// Leaf: ??_GCRetailTree_004ae7b0@@QAEPAXI@Z
// The chunk label is propagated; named by effect. Shape: no vtable, the head re-read after every store (the stores may alias it), `this` in esi, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
extern "C" void __cdecl free(void* block);        // 0x00BFEA14
struct SNode_004ae7b0 {                           // retail tree node: +4 parent, +8 left, +0xc right
    unsigned char   _pad[4];
    SNode_004ae7b0* m_pParent;                     // +0x04
    SNode_004ae7b0* m_pLeft;                       // +0x08
    SNode_004ae7b0* m_pRight;                      // +0x0c
};
struct CRetailTree_004ae7b0 {
    SNode_004ae7b0* m_pHead;                       // +0x00
    long            m_size;                        // +0x04
    void Erase(SNode_004ae7b0* node);              // 0x004AE490 (landed as CTreeOwner_004ae490::FreeTree; labelled "LTextTreeWalkThrough::BuildTreeArray")
    ~CRetailTree_004ae7b0()
    {
        if (m_size != 0) {
            Erase(m_pHead->m_pParent);
            m_pHead->m_pLeft = m_pHead;
            m_pHead->m_pParent = 0;
            m_pHead->m_pRight = m_pHead;
            m_size = 0;
        }
        if (m_pHead)
            free(m_pHead);
    }
};

void Kill_004ae7b0(CRetailTree_004ae7b0* object)                      // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}