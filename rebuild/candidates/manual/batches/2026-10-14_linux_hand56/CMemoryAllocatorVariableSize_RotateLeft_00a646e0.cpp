// CMemoryAllocatorVariableSize::RotateLeft(SVarChunk* node) -- the red-black tree rotation over the chunk tree (left child +4, right child +0x10, parent +0xc): the right child replaces the node under its parent (or becomes the root at +0xc34 of the allocator), the node becomes the child's left child
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A646E0, length 96 bytes.
// Leaf: RotateLeft
// Shape: the node in edx, the right child in eax, the three tails each ending with the same two stores and `ret 4`.
struct SVarChunk_00a646e0 {                        // the chunk tree node: left +4, parent +0xc, right +0x10
    long                m_0;
    SVarChunk_00a646e0* m_pLeft;                   // +0x04
    long                m_8;
    SVarChunk_00a646e0* m_pParent;                 // +0x0c
    SVarChunk_00a646e0* m_pRight;                  // +0x10
};

struct CMemoryAllocatorVariableSize {
    unsigned char       _pad00[0xc34];
    SVarChunk_00a646e0* m_pChunkTreeRoot;          // +0xc34
    void RotateLeft(SVarChunk_00a646e0* node);
};

void CMemoryAllocatorVariableSize::RotateLeft(SVarChunk_00a646e0* node)
{
    SVarChunk_00a646e0* child = node->m_pRight;
    node->m_pRight = child->m_pLeft;
    if (child->m_pLeft)
        child->m_pLeft->m_pParent = node;
    child->m_pParent = node->m_pParent;
    if (!node->m_pParent)
        m_pChunkTreeRoot = child;
    else if (node == node->m_pParent->m_pRight)
        node->m_pParent->m_pRight = child;
    else
        node->m_pParent->m_pLeft = child;
    child->m_pLeft = node;
    node->m_pParent = child;
}
