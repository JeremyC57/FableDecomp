// CPhysicsMesh::CalculateNavigationLineInfo() -- size the navigation-line table to the number of NAV_LAYER levels (at least one), flag the mesh as having navigation lines when any level exists, clear every level's line list, then rebuild the lines
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A74360, length 160 bytes.
// Leaf: CalculateNavigationLineInfo
// Callees: CPhysicsMesh::CalculateNumberOfNavigationLineLevels @ 0x00A73DB0, vector<CArray<C2DLineF>>::resize(n, x)
// @ 0x00A75230, CArray<C2DLineF>::erase(first, last) @ 0x00A748E0 (catalogued "Vector_EraseRange"; the inline clear()),
// the line rebuild @ 0x00A74180 (catalogued "NavMesh_ProcessLine_Indices").
// Layout: Ego_r.pdb CPhysicsMesh with the retail containers: NavigationLineListInfo +0x0c (bool), NavigationLines +0x48
// (12-byte vector of 12-byte CArray<C2DLineF>).
// Shape: one zero register (edi) for the empty-array temporary's three words and the loop index; the element count is
// the signed `/12` re-derived every pass (`jl`), the element reached as `*(begin() + i)` and cleared through its inline
// clear(); `ret`.
struct C2DLineF { float m[4]; };

struct CLineArray {                                // retail CArray<C2DLineF>: first / last / end (no destructor here)
    C2DLineF* m_pFirst;
    C2DLineF* m_pLast;
    C2DLineF* m_pEnd;
    CLineArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    C2DLineF* erase(C2DLineF* first, C2DLineF* last);   // 0x00A748E0
    void clear() { erase(m_pFirst, m_pLast); }
};
struct CLineArrayVector {                          // vector<CArray<C2DLineF>>
    CLineArray* m_pFirst;
    CLineArray* m_pLast;
    CLineArray* m_pEnd;
    CLineArray* begin() { return m_pFirst; }
    long size() const { return m_pLast - m_pFirst; }
    CLineArray& operator[](long index) { return *(begin() + index); }
    void resize(unsigned long count, const CLineArray& value);   // 0x00A75230
};

struct CPhysicsMesh {
    unsigned char    _pad00[0x0c];
    bool             NavigationLineListInfo;       // +0x0c
    unsigned char    _pad0d[0x48 - 0x0d];
    CLineArrayVector NavigationLines;              // +0x48
    long CalculateNumberOfNavigationLineLevels() const;   // 0x00A73DB0
    void ProcessNavigationLines();                 // 0x00A74180
    void CalculateNavigationLineInfo();
};

void CPhysicsMesh::CalculateNavigationLineInfo()
{
    long levels = CalculateNumberOfNavigationLineLevels();
    if (levels != 0)
        NavigationLineListInfo = true;
    if (levels < 1)
        levels = 1;
    NavigationLines.resize(levels, CLineArray());
    for (long i = 0; i < NavigationLines.size(); ++i)
        NavigationLines[i].clear();
    ProcessNavigationLines();
}