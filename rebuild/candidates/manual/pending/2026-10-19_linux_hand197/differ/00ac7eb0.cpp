// COptimisedPrimitive::ComputeSelfContraction(CVertexPairContraction&) const -- cost of collapsing an unsplit vertex pair (one position, two texture/normal seams): weighted squared texture and normal distances, plus the high penalty when the normals face apart; the pair collapses onto the first vertex at half weight
// Leaf: ComputeSelfContraction
// Grade: DIFFER(215v215) -- x87 operand order only: retail squares the normal distance before scaling it (n*n*w) and loads the dot product's z/y terms from the second vertex but the x term from the first; the lane scales first and commutes the terms uniformly (tried squared locals, both Dot directions, a mixed-order Dot)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC7EB0, 215 bytes (`ret 4`).
// Callees: none (fsqrt inline through `#pragma intrinsic(sqrt)`); 0.0f 0x0122DEDC.
// Layout: retail COptimisedPrimitive (the landed RemapGroups 0x00AC99F0): Vertices at +0x10 of 0xb8-byte COptimisedVertex
//   {Position +0, Normal +0xc, TextureCoord +0x18}; COptimisationParameters UnsplitTextureWeight +0x74, UnsplitNormalWeight
//   +0x78, HighPenalty +0x7c (Ego_r.pdb +0x90..+0x98, seven containers 4 bytes smaller); CVertexPairContraction {Exists +0,
//   Unsplit +1, SubContractions +4, Weight2 +0x10, Cost +0x14, ResultingPosition +0x18}.
// Shape: both lengths are float locals (sqrt of the squared difference), the cost a named float that the penalty test
//   adds to; ResultingPosition is the implicit C3DVector assignment (shared `lea` destination) and Weight2's 0.5f an
//   immediate store.
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
    unsigned char _pad24[4];
    CQuadric      PositionError;                   // +0x28
    float         Cost;                            // +0x80 (after the quadric's tail)
    unsigned char _rest[0xb8 - 0x54];
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
    unsigned char            _pad1c[0x58];         // Faces, Edges, VertexPairContractions, SegmentCount, SegmentSizes, SubChunksToKeep, ...
    float                    Parameters_UnsplitTextureWeight;   // +0x74
    float                    Parameters_UnsplitNormalWeight;    // +0x78
    float                    Parameters_HighPenalty;            // +0x7c
    void ComputeSelfContraction(CVertexPairContraction& contraction) const;
};

void COptimisedPrimitive::ComputeSelfContraction(CVertexPairContraction& contraction) const
{
    const CSubContraction& sub = contraction.SubContractions[0];
    const COptimisedVertex& v1 = Vertices[sub.Vertex1];
    const COptimisedVertex& v2 = Vertices[sub.Vertex2];
    float textureDistance = (v2.TextureCoord - v1.TextureCoord).GetLength();
    float normalDistance = (v2.Normal - v1.Normal).GetLength();
    textureDistance *= textureDistance;
    float cost = textureDistance * Parameters_UnsplitTextureWeight + normalDistance * normalDistance * Parameters_UnsplitNormalWeight;
    if (v2.Normal.Dot(v1.Normal) < 0.0f)
        cost += Parameters_HighPenalty;
    contraction.Cost = cost;
    contraction.ResultingPosition = v1.Position;
    contraction.Weight2 = 0.5f;
}
