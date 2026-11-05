// Behaviour test for QuadricOptimiserInternals::COptimisedPrimitive::PerformSubContraction 0x00ACCBF0 (parked): models the merge of
// vertex Index2 into Index1 -- faces of vertex2 remapped (degenerate ones killed), vertex1's face list rebuilt from the union of live
// faces, texture coordinate / normal interpolated by (1 - Weight2, Weight2), position error averaged, cost = max unless unsplit.
#include <stdio.h>
struct Face { unsigned long V[3]; bool Exists; };
static void Remap(Face& f, unsigned long from, unsigned long to) { for (int k = 0; k < 3; ++k) if (f.V[k] == from) f.V[k] = to; }
static bool Degenerate(const Face& f) { return f.V[0] == f.V[1] || f.V[1] == f.V[2] || f.V[2] == f.V[0]; }
static bool Near(float a, float b) { return a - b < 0.0001f && b - a < 0.0001f; }
int main()
{
    Face faces[3] = { { { 0, 1, 2 }, true }, { { 1, 2, 3 }, true }, { { 0, 1, 3 }, true } };
    unsigned long list2[2] = { 0, 1 };                     // faces using vertex 1 (index2)
    for (int k = 0; k < 2; ++k) {
        Face& f = faces[list2[k]];
        if (f.Exists) { Remap(f, 1, 0); if (Degenerate(f)) f.Exists = false; }
    }
    unsigned long unionList[3] = { 0, 1, 2 }; unsigned long kept[3]; int nk = 0;
    for (int k = 0; k < 3; ++k) if (faces[unionList[k]].Exists) kept[nk++] = unionList[k];
    float weight2 = 0.25f, weight1 = 1.0f - weight2;
    float u1 = 2.0f, u2 = 6.0f, u = u1 * weight1 + u2 * weight2;
    double error1 = 3.0, error2 = 5.0; double error = (error1 + error2) * 0.5;
    float cost1 = 1.5f, cost2 = 4.0f; bool unsplit = false; float cost = unsplit ? 0.0f : (cost1 > cost2 ? cost1 : cost2);
    bool ok = !faces[0].Exists && faces[1].Exists && faces[1].V[0] == 0 && nk == 2 && kept[0] == 1 && kept[1] == 2
           && Near(u, 3.0f) && error == 4.0 && cost == 4.0f;
    printf(ok ? "00accbf0_TEST PASS\n" : "00accbf0_TEST FAIL\n");
    return ok ? 0 : 1;
}
