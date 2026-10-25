// Link test for 00acd840 QuadricOptimiserInternals::COptimisedPrimitive::PerformContraction: calls the source object;
// nothing happens unless every vertex named by every sub-contraction still exists; otherwise all sub-contractions are
// performed, then all are updated, in order.
#include <cstdio>
#include <cstring>
int main();
static char g_log[64]; static int g_n = 0; static bool g_ok = true; static const void* g_c = 0;
class C3DVector { public: float X, Y, Z; };
template <class T> class CArray {
public:
    T* m_first; T* m_last; T* m_end;
    unsigned long size() const { return m_last - m_first; }
    T& operator[](unsigned long n) const { return m_first[n]; }
};
namespace QuadricOptimiserInternals {
class COptimisedVertex {
public:
    C3DVector Position, Normal, TextureCoord;
    unsigned char _pad24[0x6c];
    bool Exists;
    unsigned char _rest[0xb8 - 0x91];
};
class CSubContraction { public: unsigned long Index1; unsigned long Index2; };
class CVertexPairContraction {
public:
    bool Exists; bool Unsplit; CArray<CSubContraction> SubContractions; float Weight2; float Cost;
    C3DVector ResultingPosition; void* ListIterator;
};
class COptimisedPrimitive {
public:
    long MaterialIndex;
    unsigned char UniqueVertices[0xc];
    CArray<COptimisedVertex> Vertices;
private:
    void PerformSubContraction(const CSubContraction& sub, const CVertexPairContraction& contraction);
    void UpdateContractions(const CSubContraction& sub);
    void PerformContraction(CVertexPairContraction& contraction);
    friend int ::main();
};
void COptimisedPrimitive::PerformSubContraction(const CSubContraction& sub, const CVertexPairContraction& contraction)
{ g_log[g_n++] = 'C'; g_log[g_n++] = (char)('0' + sub.Index1); g_ok = g_ok && &contraction == g_c; }
void COptimisedPrimitive::UpdateContractions(const CSubContraction& sub)
{ g_log[g_n++] = 'U'; g_log[g_n++] = (char)('0' + sub.Index1); }
}
using namespace QuadricOptimiserInternals;
int main()
{
    if (sizeof(COptimisedVertex) != 0xb8 || sizeof(CVertexPairContraction) != 0x28) { std::printf("00acd840_TEST FAIL size\n"); return 1; }
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
