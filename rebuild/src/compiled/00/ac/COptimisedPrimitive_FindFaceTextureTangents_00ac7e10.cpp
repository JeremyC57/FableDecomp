// COptimisedPrimitive::FindFaceTextureTangents(COptimisedFace& face) -- compute the face's U and V texture tangents from its three vertices' positions and their U (then V) texture coordinates
// Leaf: FindFaceTextureTangents
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC7E10, length 155 bytes (padded to 160).
// Callees: COptimisedPrimitive::FindTextureTangent(const C3DVector&, const C3DVector&, const C3DVector&, float, float, float)
//          @ 0x00AC7300 (C3DVector by hidden pointer), twice.
// Layout: retail COptimisedPrimitive keeps its arrays as 12-byte vectors (first/last/end): Vertices at +0x10 (the engine
// header's PDB CArray layout puts it at +0x14, so the class is modelled locally). COptimisedVertex is retail 0xb8 bytes
// (PDB 0xc0 with 16-byte maps): Position +0, TextureCoord +0x18. COptimisedFace (0x48): VertexIndices +0,
// UTangent +0x24, VTangent +0x30.
// Shape: `this` is spilled to the frame (every callee-saved register holds a vertex/face pointer) and reloaded for
// the second call; both results go through one C3DVector temporary and are copied word by word.
class C3DVector { public:
    float X, Y, Z;
};
struct COptimisedVertex {                   // QuadricOptimiserInternals::COptimisedVertex (retail 0xb8 bytes)
    C3DVector     Position;                 // +0x00
    C3DVector     Normal;                   // +0x0c
    C3DVector     TextureCoord;             // +0x18 (X = U, Y = V)
    unsigned char _rest[0xb8 - 0x24];
};
struct COptimisedFace {                     // QuadricOptimiserInternals::COptimisedFace (0x48 bytes)
    unsigned long VertexIndices[3];         // +0x00
    unsigned long UniqueVertexIndices[3];   // +0x0c
    long          Edges[3];                 // +0x18
    C3DVector     UTangent;                 // +0x24
    C3DVector     VTangent;                 // +0x30
    bool          Exists;                   // +0x3c
    unsigned long OutputIndex;              // +0x40
    long          Segment;                  // +0x44
};
template <class T> struct CRetailVector {   // retail std::vector (12 bytes)
    T* m_first;
    T* m_last;
    T* m_end;
    T& operator[](unsigned long n) { return m_first[n]; }
};

class COptimisedPrimitive {
public:
    long                            MaterialIndex;      // +0x00
    unsigned char                   UniqueVertices[0xc];// +0x04
    CRetailVector<COptimisedVertex> Vertices;           // +0x10
    C3DVector FindTextureTangent(const C3DVector& p0, const C3DVector& p1, const C3DVector& p2,
                                 float t0, float t1, float t2);   // 0x00AC7300
    void FindFaceTextureTangents(COptimisedFace& face);
};

void COptimisedPrimitive::FindFaceTextureTangents(COptimisedFace& face)
{
    COptimisedVertex& v0 = Vertices[face.VertexIndices[0]];
    COptimisedVertex& v1 = Vertices[face.VertexIndices[1]];
    COptimisedVertex& v2 = Vertices[face.VertexIndices[2]];
    face.UTangent = FindTextureTangent(v0.Position, v1.Position, v2.Position,
                                       v0.TextureCoord.X, v1.TextureCoord.X, v2.TextureCoord.X);
    face.VTangent = FindTextureTangent(v0.Position, v1.Position, v2.Position,
                                       v0.TextureCoord.Y, v1.TextureCoord.Y, v2.TextureCoord.Y);
}