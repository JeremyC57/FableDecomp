// QuadricOptimiserInternals::COptimisedPrimitive::PerformContraction(CVertexPairContraction&) (private; Ego_r.pdb
// `?PerformContraction@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXAAVCVertexPairContraction@2@@Z`) -- carry out
// a pair contraction only when every vertex it names still exists: perform each sub-contraction, then update each
// sub-contraction's neighbouring contractions.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ACD840, 179 bytes (+ int3/nop padding; `ret 4`).
// Callees (Ego_r.pdb, both private on `this`): PerformSubContraction(const CSubContraction&, const
//   CVertexPairContraction&) 0x00ACCBF0 and UpdateContractions(const CSubContraction&) 0x00AC96E0.
// Layout: retail STLport 12-byte CArrays (the PDB's Dinkumware build has 16): COptimisedPrimitive Vertices at +0x10
//   (0xb8-byte COptimisedVertex records, Exists +0x90); CVertexPairContraction SubContractions at +4 of 8-byte
//   CSubContraction {Index1, Index2} records (Ego_r.pdb member names).
// Shape: three indexed walks over the sub-contractions; the existence scan takes the count once and keeps the vertex
//   array in a register (an early return out of the loop); the two work loops re-read size() after every call.
// Leaf: ?PerformContraction@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXAAVCVertexPairContraction@2@@Z
class C3DVector {
public:
    float X, Y, Z;
};
template <class T> class CArray {                  // retail CArray (STLport vector): first / last / end
public:
    T* m_first;
    T* m_last;
    T* m_end;
    unsigned long size() const { return m_last - m_first; }
    T& operator[](unsigned long n) const { return m_first[n]; }
};
namespace QuadricOptimiserInternals {
class COptimisedVertex {                           // retail 0xb8 bytes (Ego_r.pdb order, STLport maps)
public:
    C3DVector     Position;                        // +0x00
    C3DVector     Normal;                          // +0x0c
    C3DVector     TextureCoord;                    // +0x18
    unsigned char _pad24[0x6c];                    // PositionError (CQuadric) +0x28, Cost +0x80, FaceList +0x84
    bool          Exists;                          // +0x90
    unsigned char _rest[0xb8 - 0x91];              // UniqueIndex, OutputIndex, Groups, PairContractions
};
class CSubContraction {                            // Ego_r.pdb, 8 bytes
public:
    unsigned long Index1;                          // +0x00
    unsigned long Index2;                          // +0x04
};
class CVertexPairContraction {                     // retail layout (Ego_r.pdb order, a 12-byte CArray)
public:
    bool                    Exists;                // +0x00
    bool                    Unsplit;               // +0x01
    CArray<CSubContraction> SubContractions;       // +0x04
    float                   Weight2;               // +0x10
    float                   Cost;                  // +0x14
    C3DVector               ResultingPosition;     // +0x18
    void*                   ListIterator;          // +0x24
};
class COptimisedPrimitive {                        // retail layout: STLport containers are 12 bytes
public:
    long                     MaterialIndex;        // +0x00
    unsigned char            UniqueVertices[0xc];  // +0x04 CArray<CUniqueVertex>
    CArray<COptimisedVertex> Vertices;             // +0x10
private:
    void PerformSubContraction(const CSubContraction& sub, const CVertexPairContraction& contraction);   // 0x00ACCBF0
    void UpdateContractions(const CSubContraction& sub);                                                  // 0x00AC96E0
    void PerformContraction(CVertexPairContraction& contraction);
};

void COptimisedPrimitive::PerformContraction(CVertexPairContraction& contraction)
{
    unsigned long i;
    unsigned long count = contraction.SubContractions.size();
    for (i = 0; i < count; ++i) {
        const CSubContraction& sub = contraction.SubContractions[i];
        if (!Vertices[sub.Index1].Exists || !Vertices[sub.Index2].Exists)
            return;
    }
    for (i = 0; i < contraction.SubContractions.size(); ++i)
        PerformSubContraction(contraction.SubContractions[i], contraction);
    for (i = 0; i < contraction.SubContractions.size(); ++i)
        UpdateContractions(contraction.SubContractions[i]);
}
}
