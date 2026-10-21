// Grade: DIFFER(388v409) -- register allocation: retail keeps faceA's first unique vertex in ebx for the whole body and spills &faceB plus faceB's second vertex to the stack (frame 0x1c, ebp pushed late); the lane pushes ebp early to hold &faceB (frame 0x14). Logic is complete; an inline-loop HasUniqueVertex (probe c, 419) and pointer locals (probe b, 396) are further off.
// QuadricOptimiserInternals::COptimisedPrimitive::JoinAdjacentFaces(unsigned long faceA, unsigned long faceB) -- links two
// faces that share exactly one edge: finds the edge of each face whose opposite vertex is not on the other face, checks
// the shared edge runs the opposite way round and that neither edge is linked yet, then appends the edge record and
// stores each face as the other's neighbour across it
// Leaf: JoinAdjacentFaces
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC8AE0, 0x18C bytes (padded to 416), `ret 8`.
// Callees: vector<COptimisedEdge>::push_back(const COptimisedEdge&) 0x00AD5CE0 (out of line).
// Layout: retail differs from the Ego_r PDB: each CArray is 12 bytes here (first/last/end, no allocator slot), so
//   Faces._Myfirst sits at +0x1C and Edges at +0x28 (PDB: CArray 16 bytes, Faces +0x24, Edges +0x34). Face and edge
//   records match the PDB (COptimisedFace 72 bytes: UniqueVertexIndices +0xC, Edges +0x18; COptimisedEdge 16 bytes).
namespace QuadricOptimiserInternals {
struct C3DVector { float X, Y, Z; };
struct COptimisedFace {
    unsigned long VertexIndices[3];                 // +0x00
    unsigned long UniqueVertexIndices[3];           // +0x0C
    long          Edges[3];                         // +0x18
    C3DVector     UTangent;                         // +0x24
    C3DVector     VTangent;                         // +0x30
    bool          Exists;                           // +0x3C
    unsigned long OutputIndex;                      // +0x40
    long          Segment;                          // +0x44
    bool HasUniqueVertex(unsigned long v) const
    {
        return UniqueVertexIndices[0] == v || UniqueVertexIndices[1] == v || UniqueVertexIndices[2] == v;
    }
};
struct COptimisedEdge {
    unsigned long FaceA;                            // +0x00
    unsigned long FaceB;                            // +0x04
    unsigned long UniqueVertex1;                    // +0x08
    unsigned long UniqueVertex2;                    // +0x0C
};
template<class T> struct CArray {
    T* _Myfirst; T* _Mylast; T* _Myend;
    T& operator[](unsigned long i) { return _Myfirst[i]; }
    void push_back(const T& value);
};
class COptimisedPrimitive {
public:
    long                    MaterialIndex;          // +0x00
    CArray<unsigned long>   UniqueVertices;         // +0x04 (element type not modelled)
    CArray<unsigned long>   Vertices;               // +0x10 (element type not modelled)
    CArray<COptimisedFace>  Faces;                  // +0x1C
    CArray<COptimisedEdge>  Edges;                  // +0x28
    bool JoinAdjacentFaces(unsigned long faceA, unsigned long faceB);
};
bool COptimisedPrimitive::JoinAdjacentFaces(unsigned long faceA, unsigned long faceB)
{
    COptimisedFace& a = Faces[faceA];
    COptimisedFace& b = Faces[faceB];
    if (b.HasUniqueVertex(a.UniqueVertexIndices[0]) && b.HasUniqueVertex(a.UniqueVertexIndices[1])
        && b.HasUniqueVertex(a.UniqueVertexIndices[2]))
        return false;
    long edgeA = -1;
    if (!b.HasUniqueVertex(a.UniqueVertexIndices[2])) edgeA = 0;
    if (!b.HasUniqueVertex(a.UniqueVertexIndices[0])) edgeA = 1;
    if (!b.HasUniqueVertex(a.UniqueVertexIndices[1])) edgeA = 2;
    long edgeB = -1;
    if (!a.HasUniqueVertex(b.UniqueVertexIndices[2])) edgeB = 0;
    if (!a.HasUniqueVertex(b.UniqueVertexIndices[0])) edgeB = 1;
    if (!a.HasUniqueVertex(b.UniqueVertexIndices[1])) edgeB = 2;
    if (edgeA == -1 || edgeB == -1)
        return false;
    if (a.UniqueVertexIndices[edgeA] != b.UniqueVertexIndices[(edgeB + 1) % 3])
        return false;
    if (a.Edges[edgeA] != -1 || b.Edges[edgeB] != -1)
        return false;
    COptimisedEdge edge;
    edge.FaceA = faceA;
    edge.FaceB = faceB;
    edge.UniqueVertex1 = a.UniqueVertexIndices[edgeA];
    edge.UniqueVertex2 = a.UniqueVertexIndices[(edgeA + 1) % 3];
    Edges.push_back(edge);
    a.Edges[edgeA] = faceB;
    b.Edges[edgeB] = faceA;
    return true;
}
}
