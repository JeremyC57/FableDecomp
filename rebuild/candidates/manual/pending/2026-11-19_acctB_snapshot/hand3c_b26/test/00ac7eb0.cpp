// Behaviour test for 0x00AC7EB0 QuadricOptimiserInternals::COptimisedPrimitive::ComputeSelfContraction (links the REAL row object).
// The row's class is declared here with its one private member (?ComputeSelfContraction@COptimisedPrimitive@QuadricOptimiserInternals@@
// AAEXAAVCVertexPairContraction@2@@Z); the objects are raw native-layout storage, every offset read by an instruction of the row:
//   primitive: +0x10 Vertices base (0x00AC7EB1), Parameters.UnsplitTextureWeight +0x74 (0x00AC7F31), UnsplitNormalWeight +0x78
//     (0x00AC7F38), HighPenalty +0x7C (0x00AC7F62);
//   vertex (0xB8 bytes, 0x00AC7EC1): Position +0x00 (0x00AC7F68), Normal +0x0C (0x00AC7F01), TextureCoord +0x18 (0x00AC7ECD);
//   contraction: +0x04 SubContractions pointer, element {Index1 +0, Index2 +4} (0x00AC7EB9..0x00AC7EBE), Weight2 +0x10 (0x00AC7F78),
//     Cost +0x14 (0x00AC7F65), ResultingPosition +0x18 (0x00AC7F6A).
// Reference model: cost = |t2 - t1|^2 * UnsplitTextureWeight + |n2 - n1|^2 * UnsplitNormalWeight, + HighPenalty when n1 . n2 < 0
// (unordered / equal / greater: no penalty); ResultingPosition = vertex1.Position; Weight2 = 0.5. Evaluated in double (the row keeps
// every intermediate on the x87 stack) and compared after the single float store, with a 1-ulp-scale tolerance.
#include <stdio.h>
#include <string.h>
#include <math.h>

namespace QuadricOptimiserInternals { class CVertexPairContraction; class COptimisedPrimitive; }
bool Run(QuadricOptimiserInternals::COptimisedPrimitive*, QuadricOptimiserInternals::CVertexPairContraction*);
namespace QuadricOptimiserInternals {
class COptimisedPrimitive {
    friend bool ::Run(COptimisedPrimitive*, CVertexPairContraction*);
    void ComputeSelfContraction(CVertexPairContraction& contraction);
};
}
using namespace QuadricOptimiserInternals;
bool Run(COptimisedPrimitive* p, CVertexPairContraction* c) { p->ComputeSelfContraction(*c); return true; }

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }
template<class T> static void put(void* base, int off, T v) { memcpy((unsigned char*)base + off, &v, sizeof v); }
template<class T> static T get(const void* base, int off) { T v; memcpy(&v, (const unsigned char*)base + off, sizeof v); return v; }

enum { NV = 6, VSIZE = 0xB8 };
static union { unsigned char b[NV * VSIZE]; double align; } verts;
static union { unsigned char b[0x100]; double align; } prim;
static union { unsigned char b[0x40]; double align; } contr;
static unsigned long subs[4];

struct V3 { float x, y, z; };
static void SetVertex(int i, V3 pos, V3 normal, V3 tex)
{
    unsigned char* v = verts.b + i * VSIZE;
    put(v, 0x00, pos.x); put(v, 0x04, pos.y); put(v, 0x08, pos.z);
    put(v, 0x0c, normal.x); put(v, 0x10, normal.y); put(v, 0x14, normal.z);
    put(v, 0x18, tex.x); put(v, 0x1c, tex.y); put(v, 0x20, tex.z);
}
static V3 Vv(float x, float y, float z) { V3 r = { x, y, z }; return r; }

static double Mag2(V3 a, V3 b) { double dx = (double)a.x - b.x, dy = (double)a.y - b.y, dz = (double)a.z - b.z; double m = sqrt(dz * dz + dy * dy + dx * dx); return m * m; }

static void Case(int i1, int i2, float wt, float wn, float hp, int k)
{
    memset(prim.b, 0x77, sizeof prim.b); memset(contr.b, 0x66, sizeof contr.b);
    put<void*>(prim.b, 0x10, verts.b);
    put(prim.b, 0x74, wt); put(prim.b, 0x78, wn); put(prim.b, 0x7c, hp);
    subs[0] = (unsigned long)i1; subs[1] = (unsigned long)i2; subs[2] = 4; subs[3] = 5;   // second sub-contraction: never read
    put<void*>(contr.b, 0x04, subs);
    unsigned char before[0x40]; memcpy(before, contr.b, sizeof before);
    unsigned char pbefore[0x100]; memcpy(pbefore, prim.b, sizeof pbefore);
    unsigned char vbefore[NV * VSIZE]; memcpy(vbefore, verts.b, sizeof vbefore);
    Run((COptimisedPrimitive*)(void*)prim.b, (CVertexPairContraction*)(void*)contr.b);

    const unsigned char* a = verts.b + i1 * VSIZE; const unsigned char* b = verts.b + i2 * VSIZE;
    V3 t1 = Vv(get<float>(a, 0x18), get<float>(a, 0x1c), get<float>(a, 0x20)), t2 = Vv(get<float>(b, 0x18), get<float>(b, 0x1c), get<float>(b, 0x20));
    V3 n1 = Vv(get<float>(a, 0x0c), get<float>(a, 0x10), get<float>(a, 0x14)), n2 = Vv(get<float>(b, 0x0c), get<float>(b, 0x10), get<float>(b, 0x14));
    double cost = Mag2(t2, t1) * wt + Mag2(n2, n1) * wn;
    double dot = (double)n1.z * n2.z + (double)n1.y * n2.y + (double)n1.x * n2.x;
    if (dot < 0.0) cost += hp;
    float want = (float)cost, got = get<float>(contr.b, 0x14);
    check(fabs((double)got - want) <= 1e-6 * (fabs(want) + 1.0), "Cost", k);
    check(get<float>(contr.b, 0x10) == 0.5f, "Weight2 = 0.5", k);
    check(memcmp(contr.b + 0x18, a, 12) == 0, "ResultingPosition = vertex1.Position", k);
    check(memcmp(contr.b, before, 0x10) == 0 && memcmp(contr.b + 0x24, before + 0x24, 0x40 - 0x24) == 0, "nothing else in the contraction written", k);
    check(memcmp(prim.b, pbefore, sizeof pbefore) == 0 && memcmp(verts.b, vbefore, sizeof vbefore) == 0, "primitive and vertices untouched", k);
}

static unsigned long seed = 99;
static unsigned long Rnd() { seed = seed * 1103515245ul + 12345ul; return (seed >> 8) & 0xffffff; }
static float RndF(float lo, float hi) { return lo + (hi - lo) * (float)(Rnd() % 4096) / 4096.0f; }

int main()
{
    int k = 0;
    // Distinct position / normal / texture per vertex; weights distinct so a swapped weight or member shows.
    SetVertex(0, Vv(1, 2, 3), Vv(0, 0, 1), Vv(0.25f, 0.5f, 0));
    SetVertex(1, Vv(-4, 5, 6), Vv(0, 1, 0), Vv(0.75f, 0.125f, 0.5f));          // normals orthogonal: dot == 0 -> no penalty
    SetVertex(2, Vv(7, -8, 9), Vv(0, 0, -1), Vv(0.25f, 0.5f, 0));              // opposite to 0: penalty; same tex as 0
    SetVertex(3, Vv(10, 11, -12), Vv(0.6f, 0.8f, 0), Vv(2.0f, -1.0f, 3.0f));
    SetVertex(4, Vv(0, 0, 0), Vv(0, 0, 1), Vv(0, 0, 0));
    SetVertex(5, Vv(0.5f, 0.5f, 0.5f), Vv(0.0f, 0.0f, 1.0f), Vv(0.0f, 0.0f, 3.0f));   // same normal as 4: normal term 0; tex distance 3
    Case(0, 1, 2.0f, 3.0f, 100.0f, ++k);    // dot exactly 0: no penalty
    Case(1, 0, 2.0f, 3.0f, 100.0f, ++k);    // swapped pair: same cost, other ResultingPosition
    Case(0, 2, 2.0f, 3.0f, 100.0f, ++k);    // dot -1: penalty; tex term 0
    Case(2, 0, 5.0f, 0.25f, 7.0f, ++k);
    Case(0, 3, 2.0f, 3.0f, 100.0f, ++k);    // dot 0 again (n3.z = 0)
    Case(3, 1, 1.5f, 0.0f, 100.0f, ++k);    // dot 0.8 > 0
    Case(4, 5, 2.0f, 3.0f, 100.0f, ++k);    // tex only: 9 * 2 = 18 (a missing square gives 6)
    Case(5, 4, 0.0f, 3.0f, 100.0f, ++k);
    Case(0, 2, 2.0f, 3.0f, 0.0f, ++k);      // penalty 0
    Case(3, 3, 2.0f, 3.0f, 100.0f, ++k);    // a vertex with itself: cost 0
    { // normal term alone: |(0,0,1) - (0,1,0)|^2 = 2 -> 2 * 3 = 6 (unsquared: 4.24)
      SetVertex(4, Vv(0, 0, 0), Vv(0, 0, 1), Vv(1, 1, 1)); SetVertex(5, Vv(1, 1, 1), Vv(0, 1, 0), Vv(1, 1, 1));
      Case(4, 5, 11.0f, 3.0f, 100.0f, ++k); }
    { // tiny negative dot -> penalty; tiny positive -> none
      SetVertex(4, Vv(0, 0, 0), Vv(1, 0, 0), Vv(0, 0, 0)); SetVertex(5, Vv(0, 0, 0), Vv(-1e-6f, 1, 0), Vv(0, 0, 0));
      Case(4, 5, 1.0f, 1.0f, 50.0f, ++k);
      SetVertex(5, Vv(0, 0, 0), Vv(1e-6f, 1, 0), Vv(0, 0, 0));
      Case(4, 5, 1.0f, 1.0f, 50.0f, ++k); }
    for (int r = 0; r < 4000; ++r) {
        for (int i = 0; i < NV; ++i)
            SetVertex(i, Vv(RndF(-50, 50), RndF(-50, 50), RndF(-50, 50)), Vv(RndF(-1, 1), RndF(-1, 1), RndF(-1, 1)),
                      Vv(RndF(-4, 4), RndF(-4, 4), RndF(-4, 4)));
        Case((int)(Rnd() % NV), (int)(Rnd() % NV), RndF(0, 10), RndF(0, 10), RndF(0, 1000), 1000 + r);
    }
    if (failures) { printf("00ac7eb0_TEST FAIL %d\n", failures); return 1; }
    printf("00ac7eb0_TEST PASS (%d targeted, 4000 random)\n", k);
    return 0;
}
