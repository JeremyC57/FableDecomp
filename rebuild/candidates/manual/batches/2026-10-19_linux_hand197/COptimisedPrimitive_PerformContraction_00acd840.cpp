// COptimisedPrimitive::PerformContraction(CVertexPairContraction&) -- carry out a pair contraction only when every vertex it names still exists: contract each sub-pair, then update each sub-pair's neighbourhood
// Leaf: PerformContraction
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ACD840, 179 bytes (+ int3/nop padding; `ret 4`).
// Callees: the sub-contraction worker 0x00ACCBF0 (sub, contraction; manifest "QuadricOptimiser_ContractVertexPair_Full") and the
//   per-sub-pair follow-up 0x00AC96E0 (sub; manifest "QuadricOptimiser_ProcessVertexContraction"), both on `this`.
// Layout: as ComputeSelfContraction 0x00AC7EB0: Vertices at +0x10 (0xb8-byte records, Exists +0x90); the contraction's
//   SubContractions CArray at +4 of 8-byte {Vertex1, Vertex2} records.
// Shape: three indexed walks over the sub-contractions; the existence scan takes the count once and keeps the vertex array in a
//   register (an early return out of the loop); the two work loops re-read size() after every call.
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
};
template <class T> struct CArray {                 // retail CArray (STLport vector): first / last / end
    T* m_first;
    T* m_last;
    T* m_end;
    unsigned long size() const { return m_last - m_first; }
    T& operator[](unsigned long n) const { return m_first[n]; }
};
namespace QuadricOptimiserInternals {
struct CQuadric { float Q[10]; };                  // 40 bytes
struct COptimisedVertex {                          // retail 0xb8 bytes (Ego_r.pdb order, STLport maps)
    C3DVector     Position;                        // +0x00
    C3DVector     Normal;                          // +0x0c
    C3DVector     TextureCoord;                    // +0x18
    unsigned char _pad24[0x6c];                    // PositionError (a 0x58-byte CQuadric at +0x28), Cost +0x80, FaceList +0x84
    bool          Exists;                          // +0x90
    unsigned char _rest[0xb8 - 0x91];
};
struct CSubContraction {
    unsigned long Vertex1;                         // +0x00
    unsigned long Vertex2;                         // +0x04
};
struct CVertexPairContraction {                    // retail layout (Ego_r.pdb order, a 12-byte CArray)
    bool                    Exists;                // +0x00
    bool                    Unsplit;               // +0x01
    CArray<CSubContraction> SubContractions;       // +0x04
    float                   Weight2;               // +0x10
    float                   Cost;                  // +0x14
    C3DVector               ResultingPosition;     // +0x18
    void*                   ListIterator;          // +0x24
};
}
using namespace QuadricOptimiserInternals;
class COptimisedPrimitive {                        // retail layout: STLport containers are 12 bytes
public:
    long                     MaterialIndex;        // +0x00
    unsigned char            UniqueVertices[0xc];  // +0x04
    CArray<COptimisedVertex> Vertices;             // +0x10
    void ContractSubPair_00accbf0(const CSubContraction& sub, CVertexPairContraction& contraction);   // 0x00ACCBF0
    void UpdateSubPair_00ac96e0(const CSubContraction& sub);                                           // 0x00AC96E0
    void PerformContraction(CVertexPairContraction& contraction);
};

void COptimisedPrimitive::PerformContraction(CVertexPairContraction& contraction)
{
    unsigned long i;
    unsigned long count = contraction.SubContractions.size();
    for (i = 0; i < count; ++i) {
        const CSubContraction& sub = contraction.SubContractions[i];
        if (!Vertices[sub.Vertex1].Exists || !Vertices[sub.Vertex2].Exists)
            return;
    }
    for (i = 0; i < contraction.SubContractions.size(); ++i)
        ContractSubPair_00accbf0(contraction.SubContractions[i], contraction);
    for (i = 0; i < contraction.SubContractions.size(); ++i)
        UpdateSubPair_00ac96e0(contraction.SubContractions[i]);
}
