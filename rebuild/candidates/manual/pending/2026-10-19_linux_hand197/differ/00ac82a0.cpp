// COptimisedPrimitive::ComputeSubContractionCost(const CSubContraction&, const C3DVector&) const -- cost of moving a vertex pair to a position: half the summed quadric error there, plus the weighted smoothing-group difference, plus the dearer of the two vertices' own costs, plus the high penalty when their normals face apart
// Leaf: ComputeSubContractionCost
// Grade: DIFFER(192v192) -- prologue scheduling only: retail pushes ebx and ebp before spilling this ([esp+8]), the lane spills this between the two pushes ([esp+4]); the rest is byte-identical (fixed the dot-product operand order)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC82A0, 192 bytes (`ret 8`), float result on the x87 stack.
// Callees: CQuadric::operator+=(const CQuadric&) 0x00AC6E80 (landed), CQuadric::Evaluate(const C3DVector&) 0x00AC6D50 (a double;
//   manifest "QuadraticForm_Evaluate"), the vertex group-difference walk 0x00AC7FF0 (on the first vertex with the second;
//   manifest "BinaryTree_ComputeSubContractionCost" is a guess); 0.5 (double) 0x012316F0, 0.0f 0x0122DEDC.
// Layout: as ComputeSelfContraction 0x00AC7EB0: Vertices at +0x10 (0xb8-byte records: Normal +0xc, PositionError +0x28 (a
//   0x58-byte CQuadric), Cost +0x80); COptimisationParameters GroupWeight +0x68, HighPenalty +0x7c (Ego_r.pdb +0x84 / +0x98).
// Shape: the summed quadric is a 0x58-byte frame copy (`rep movsd`) of the first vertex's then `+=` the second's; the running
//   cost is a float homed in the dead position-argument slot; the max is a ternary.
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
struct CQuadric {                                  // eleven doubles, 0x58 bytes (landed operator+= 0x00AC6E80)
    double AA, AB, AC, AD, BB, BC, BD, CC, CD, DD, Weight;
    CQuadric& operator+=(const CQuadric& other);   // 0x00AC6E80
    double Evaluate(const C3DVector& pos) const;   // 0x00AC6D50
};
struct COptimisedVertex {                          // retail 0xb8 bytes (Ego_r.pdb order, STLport maps)
    C3DVector     Position;                        // +0x00
    C3DVector     Normal;                          // +0x0c
    C3DVector     TextureCoord;                    // +0x18
    unsigned char _pad24[4];
    CQuadric      PositionError;                   // +0x28
    float         Cost;                            // +0x80
    unsigned char _rest[0xb8 - 0x84];
    float GetGroupDifference_00ac7ff0(const COptimisedVertex& other) const;   // 0x00AC7FF0 (walks the Groups map at +0x9c)
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
    unsigned char            _pad1c[0x4c];         // Faces, Edges, VertexPairContractions, SegmentCount, SegmentSizes, SubChunksToKeep, ...
    float                    Parameters_GroupWeight;            // +0x68
    float                    Parameters_TextureWeight;          // +0x6c
    float                    Parameters_TexturePenalty;         // +0x70
    float                    Parameters_UnsplitTextureWeight;   // +0x74
    float                    Parameters_UnsplitNormalWeight;    // +0x78
    float                    Parameters_HighPenalty;            // +0x7c
    float ComputeSubContractionCost(const CSubContraction& sub, const C3DVector& pos) const;
};

float COptimisedPrimitive::ComputeSubContractionCost(const CSubContraction& sub, const C3DVector& pos) const
{
    const COptimisedVertex& v1 = Vertices[sub.Vertex1];
    const COptimisedVertex& v2 = Vertices[sub.Vertex2];
    CQuadric error = v1.PositionError;
    error += v2.PositionError;
    float cost = (float)(error.Evaluate(pos) * 0.5);
    cost = v1.GetGroupDifference_00ac7ff0(v2) * Parameters_GroupWeight + cost;
    cost += (v1.Cost > v2.Cost) ? v1.Cost : v2.Cost;
    if (v1.Normal.Dot(v2.Normal) < 0.0f)
        cost += Parameters_HighPenalty;
    return cost;
}
