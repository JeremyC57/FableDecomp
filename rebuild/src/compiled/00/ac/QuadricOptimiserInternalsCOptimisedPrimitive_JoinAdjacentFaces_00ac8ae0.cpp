// QuadricOptimiserInternals::COptimisedPrimitive::JoinAdjacentFaces(unsigned long faceA, unsigned long faceB) -- links two faces
// that share exactly one edge. Rejects the pair when face B holds all three unique vertices of face A; otherwise finds on each
// face the edge whose opposite vertex is missing from the other face (edge i runs U[i] -> U[(i+1)%3], its opposite vertex is
// U[(i+2)%3]), requires the shared edge to run the opposite way round on B (a.U[edgeA] == b.U[(edgeB+1)%3]) and neither edge
// slot to be linked yet (-1), then appends the edge record {faceA, faceB, a.U[edgeA], a.U[(edgeA+1)%3]} to Edges and stores
// each face index as the other's neighbour across that edge. Returns true when the faces were joined.
// Leaf: ?JoinAdjacentFaces@COptimisedPrimitive@QuadricOptimiserInternals@@AAE_NKK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC8AE0, 409 bytes, __fastcall, `ret 8` (Ego_r.pdb 0x00C61CA0, FableWin.pdb same decoration).
// Callees: CArray<COptimisedEdge>::push_back(const COptimisedEdge&) 0x00AD5CE0 (out of line in retail, body visible:
//   FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE + FABLE_RETAIL_STLP_VECTOR). COptimisedFace::HasUniqueVertex is inline (expanded six times).
// Shape: no frame pointer; frame 1Ch = this spill, &b spill, b.U[1] spill and the 16-byte edge temporary. The visible push_back
//   body is what lets VC7.1 keep &a in esi and edgeA/edgeB in ebp/edi across the call and reload faceA/faceB from the argument
//   slots; the indices are signed (`cdq ; idiv` for % 3). Faces at +1Ch (72-byte records), Edges at +28h.
// Headers: overlay opt-ins FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_JOINADJACENTFACES (declaration),
//   FABLE_RETAIL_QUADRIC_COPTIMISEDFACE_HASUNIQUEVERTEX (inline accessor), FABLE_RETAIL_QUADRIC_COPTIMISEDEDGE (PDB record).
#define FABLE_RETAIL_STLP_VECTOR

#define FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_QUADRIC_COPTIMISEDFACE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDFACE_HASUNIQUEVERTEX
#define FABLE_RETAIL_QUADRIC_COPTIMISEDEDGE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_JOINADJACENTFACES
#include "engine/QuadricOptimiserTypes.h"
#include "engine/COptimisedPrimitive.h"
namespace QuadricOptimiserInternals {
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