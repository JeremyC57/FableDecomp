#include <cstdio>
#include <cstring>
// Behaviour test for 00acd840 COptimisedPrimitive::PerformContraction: nothing happens unless every vertex named by every
// sub-contraction still exists; otherwise all sub-pairs are contracted, then all are updated, in order.
static char g_log[64]; static int g_n = 0; static bool g_ok = true; static const void* g_c = 0;
#include <cmath>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const { return (float)std::sqrt(X * X + Y * Y + Z * Z); }
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
    void ContractSubPair_00accbf0(const CSubContraction& sub, CVertexPairContraction& contraction) { g_log[g_n++] = 'C'; g_log[g_n++] = (char)('0' + sub.Vertex1); g_ok = g_ok && &contraction == g_c; }
    void UpdateSubPair_00ac96e0(const CSubContraction& sub) { g_log[g_n++] = 'U'; g_log[g_n++] = (char)('0' + sub.Vertex1); }
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

int main()
{
    static COptimisedPrimitive prim;
    static COptimisedVertex verts[6];
    for (int i = 0; i < 6; ++i) verts[i].Exists = true;
    prim.Vertices.m_first = verts; prim.Vertices.m_last = verts + 6; prim.Vertices.m_end = verts + 6;
    CSubContraction subs[3] = { { 1, 2 }, { 3, 4 }, { 5, 0 } };
    CVertexPairContraction c; c.SubContractions.m_first = subs; c.SubContractions.m_last = subs + 3; c.SubContractions.m_end = subs + 3;
    g_c = &c;
    prim.PerformContraction(c);
    g_log[g_n] = 0;
    bool ok = g_ok && std::strcmp(g_log, "C1C3C5U1U3U5") == 0;
    verts[4].Exists = false; g_n = 0;
    prim.PerformContraction(c);
    ok = ok && g_n == 0;
    if (!ok) { std::printf("00acd840_TEST FAIL\n"); return 1; }
    std::printf("00acd840_TEST PASS\n");
    return 0;
}