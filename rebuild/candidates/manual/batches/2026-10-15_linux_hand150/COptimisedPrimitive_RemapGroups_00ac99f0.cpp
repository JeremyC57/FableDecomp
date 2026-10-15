// COptimisedPrimitive::RemapGroups(const CGroupRemap& remap) -- remap the smoothing-group ids of every vertex through the given table
// Leaf: RemapGroups
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC99F0, length 104 bytes (padded to 112).
// Callees: COptimisedVertex::RemapGroups(const CGroupRemap&) @ 0x00AC97D0 (manifest "QuadricOptimiser_RemapAndAccumulateCosts",
//          an unattributed name; `this` is the vertex, one stack argument).
// Layout: retail COptimisedPrimitive::Vertices is a 12-byte vector at +0x10 of 0xb8-byte COptimisedVertex records
// (the `imul 0xb21642c9 ; sar 7` division by 0xb8).
// Shape: `for (unsigned long i = 0; i < Vertices.size(); ++i)` -- size() re-evaluated every pass (signed pointer
// difference / sizeof, unsigned `jb` compare), the element address strength-reduced to a running byte offset.
struct CGroupRemap;
struct COptimisedVertex {                   // QuadricOptimiserInternals::COptimisedVertex (retail 0xb8 bytes)
    unsigned char _body[0xb8];
    void RemapGroups(const CGroupRemap& remap);     // 0x00AC97D0
};
template <class T> struct CRetailVector {   // retail std::vector (12 bytes)
    T* m_first;
    T* m_last;
    T* m_end;
    unsigned long size() const { return m_last - m_first; }
    T& operator[](unsigned long n) { return m_first[n]; }
};

class COptimisedPrimitive {
public:
    long                            MaterialIndex;      // +0x00
    unsigned char                   UniqueVertices[0xc];// +0x04
    CRetailVector<COptimisedVertex> Vertices;           // +0x10
    void RemapGroups(const CGroupRemap& remap);
};

void COptimisedPrimitive::RemapGroups(const CGroupRemap& remap)
{
    for (unsigned long i = 0; i < Vertices.size(); ++i)
        Vertices[i].RemapGroups(remap);
}
