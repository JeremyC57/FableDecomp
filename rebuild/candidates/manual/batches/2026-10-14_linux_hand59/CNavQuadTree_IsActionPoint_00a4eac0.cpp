// CNavQuadTree::IsActionPoint(long key) -- true when the key is in the action-point tree at +0x20 (the tree's Find @ 0x00A54310 returning the head when absent)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4EAC0, length 32 bytes.
// Leaf: IsActionPoint
// Shape: the tree's address in esi for the compare with its head, `setne al`, `ret 4`.
struct CRetailTree_00a4eac0 {
    void* m_pHead;                                 // +0x00
    void* Find(long key);                          // 0x00A54310
};

struct CNavQuadTree {
    unsigned char        _pad00[0x20];
    CRetailTree_00a4eac0 m_actionPoints;           // +0x20
    bool IsActionPoint(long key);
};

bool CNavQuadTree::IsActionPoint(long key)
{
    return m_actionPoints.Find(key) != m_actionPoints.m_pHead ? true : false;
}
