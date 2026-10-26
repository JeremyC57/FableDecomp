// CNavigatorManager::GetNodeAt(const C2DVector& position, bool layer) -- the navigation node under the position: the first map on the list at +0x24 whose integer bounds (+0x14..+0x20) contain the truncated coordinates answers through its tree (0x00A770B0 with (&position, layer)); null when none does
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A14F70, length 96 bytes.
// Leaf: GetNodeAt
// Callees: __ftol2 @ 0x00BFEA70 (the two coordinates truncated once, after the empty test), the tree's member. Shape: the sentinel in ebx, the node in esi, the integer coordinates in edi / eax, `ret`.
class C2DVector { public: float X, Y; };
class CANavQuadTreeNode;
class CNavQuadTree { public:
    CANavQuadTreeNode* GetNodeAt(const C2DVector* position, long layer);   // 0x00A770B0 (the bool widened with movzx)
};
struct STreeNode_00a14f70 {                           // the navigation-map list node: id at +0x10, the tree at +8, the integer bounds at +0x14..+0x20
    STreeNode_00a14f70* m_pNext;                      // +0x00
    STreeNode_00a14f70* m_pPrev;                      // +0x04
    CNavQuadTree*    m_pTree;                      // +0x08
    long             m_c;
    long             m_id;                         // +0x10
    long             m_minX;                       // +0x14
    long             m_minY;                       // +0x18
    long             m_maxX;                       // +0x1c
    long             m_maxY;                       // +0x20
};

class CNavigatorManager { public:
    unsigned char    _pad00[0x24];
    STreeNode_00a14f70* m_pMaps;                      // +0x24: the sentinel
    CANavQuadTreeNode* GetNodeAt(const C2DVector& position, bool layer);
};

CANavQuadTreeNode* CNavigatorManager::GetNodeAt(const C2DVector& position, bool layer)
{
    for (STreeNode_00a14f70* node = m_pMaps->m_pNext; node != m_pMaps; node = node->m_pNext) {
        long x = (long)position.X;
        long y = (long)position.Y;
        if (x >= node->m_minX && x < node->m_maxX && y >= node->m_minY && y < node->m_maxY)
            return node->m_pTree->GetNodeAt(&position, layer);
    }
    return 0;
}