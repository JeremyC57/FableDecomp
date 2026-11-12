// Behaviour test for 0x00AC8AE0 QuadricOptimiserInternals::COptimisedPrimitive::JoinAdjacentFaces (copy of the model and logic).
#include <cstdio>
struct Face {
    unsigned long VertexIndices[3];
    unsigned long UniqueVertexIndices[3];
    long Edges[3];
    bool HasUniqueVertex(unsigned long v) const
    { return UniqueVertexIndices[0] == v || UniqueVertexIndices[1] == v || UniqueVertexIndices[2] == v; }
};
struct Edge { unsigned long FaceA, FaceB, UniqueVertex1, UniqueVertex2; };
struct Primitive {
    Face Faces[8];
    Edge Edges[8];
    int EdgeCount;
    bool JoinAdjacentFaces(unsigned long faceA, unsigned long faceB)
    {
        Face& a = Faces[faceA];
        Face& b = Faces[faceB];
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
        Edge edge;
        edge.FaceA = faceA;
        edge.FaceB = faceB;
        edge.UniqueVertex1 = a.UniqueVertexIndices[edgeA];
        edge.UniqueVertex2 = a.UniqueVertexIndices[(edgeA + 1) % 3];
        Edges[EdgeCount++] = edge;
        a.Edges[edgeA] = faceB;
        b.Edges[edgeB] = faceA;
        return true;
    }
};
static void SetFace(Face& f, unsigned long u0, unsigned long u1, unsigned long u2)
{
    f.UniqueVertexIndices[0] = u0; f.UniqueVertexIndices[1] = u1; f.UniqueVertexIndices[2] = u2;
    f.VertexIndices[0] = u0 + 100; f.VertexIndices[1] = u1 + 100; f.VertexIndices[2] = u2 + 100;
    f.Edges[0] = f.Edges[1] = f.Edges[2] = -1;
}
static bool Untouched(const Face& f) { return f.Edges[0] == -1 && f.Edges[1] == -1 && f.Edges[2] == -1; }
int main()
{
    static Primitive p;
    bool ok = true;
    p.EdgeCount = 0;
    // 1: faces {1,2,3} and {3,2,4} share edge 2-3 with opposite winding: edgeA = 1 (2->3), edgeB = 0 (3->2).
    SetFace(p.Faces[0], 1, 2, 3); SetFace(p.Faces[1], 3, 2, 4);
    if (!p.JoinAdjacentFaces(0, 1)) ok = false;
    if (p.EdgeCount != 1) ok = false;
    if (p.Edges[0].FaceA != 0 || p.Edges[0].FaceB != 1 || p.Edges[0].UniqueVertex1 != 2 || p.Edges[0].UniqueVertex2 != 3) ok = false;
    if (p.Faces[0].Edges[0] != -1 || p.Faces[0].Edges[1] != 1 || p.Faces[0].Edges[2] != -1) ok = false;
    if (p.Faces[1].Edges[0] != 0 || p.Faces[1].Edges[1] != -1 || p.Faces[1].Edges[2] != -1) ok = false;
    // 2: joining the same pair again fails (both slots are linked) and appends nothing.
    if (p.JoinAdjacentFaces(0, 1)) ok = false;
    if (p.EdgeCount != 1) ok = false;
    // 3: the wrap-around edge: {3,1,2} v {3,2,4}: edgeA = 2 (2->3 through index 0), edgeB = 0.
    SetFace(p.Faces[2], 3, 1, 2); SetFace(p.Faces[3], 3, 2, 4);
    if (!p.JoinAdjacentFaces(2, 3)) ok = false;
    if (p.EdgeCount != 2) ok = false;
    if (p.Edges[1].FaceA != 2 || p.Edges[1].FaceB != 3 || p.Edges[1].UniqueVertex1 != 2 || p.Edges[1].UniqueVertex2 != 3) ok = false;
    if (p.Faces[2].Edges[2] != 3 || p.Faces[2].Edges[0] != -1 || p.Faces[2].Edges[1] != -1) ok = false;
    if (p.Faces[3].Edges[0] != 2) ok = false;
    // 4: same winding on both faces ({1,2,3} v {2,3,4}) is rejected.
    SetFace(p.Faces[4], 1, 2, 3); SetFace(p.Faces[5], 2, 3, 4);
    if (p.JoinAdjacentFaces(4, 5)) ok = false;
    if (!Untouched(p.Faces[4]) || !Untouched(p.Faces[5])) ok = false;
    // 5: identical unique vertices (any rotation) are rejected at once.
    SetFace(p.Faces[5], 3, 1, 2);
    if (p.JoinAdjacentFaces(4, 5)) ok = false;
    // 6: disjoint faces: edgeA = edgeB = 2, a.U[2] = 3 != b.U[0] = 4.
    SetFace(p.Faces[5], 4, 5, 6);
    if (p.JoinAdjacentFaces(4, 5)) ok = false;
    // 7: only face B's slot already linked: rejected, nothing written.
    SetFace(p.Faces[5], 3, 2, 4); p.Faces[5].Edges[0] = 7;
    if (p.JoinAdjacentFaces(4, 5)) ok = false;
    if (!Untouched(p.Faces[4]) || p.Faces[5].Edges[0] != 7) ok = false;
    // 8: B's last vertex is the odd one out on the other side: {1,2,3} v {2,1,9}: edgeA = 0 (1->2), edgeB = 0 (2->1).
    SetFace(p.Faces[6], 1, 2, 3); SetFace(p.Faces[7], 2, 1, 9);
    if (!p.JoinAdjacentFaces(6, 7)) ok = false;
    if (p.EdgeCount != 3 || p.Edges[2].UniqueVertex1 != 1 || p.Edges[2].UniqueVertex2 != 2) ok = false;
    if (p.Faces[6].Edges[0] != 7 || p.Faces[7].Edges[0] != 6) ok = false;
    if (!ok) { std::printf("00ac8ae0_TEST FAIL\n"); return 1; }
    std::printf("00ac8ae0_TEST PASS\n");
    return 0;
}