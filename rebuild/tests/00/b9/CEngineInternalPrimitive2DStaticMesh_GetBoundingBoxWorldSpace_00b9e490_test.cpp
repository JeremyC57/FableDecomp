#include <cstdio>
class CMatrix3x4 { public: float m_rows[12]; }; static const CMatrix3x4* g_m = 0; static float g_min0 = -1;
class C3DBoundingBox { public: float Min[3]; float Max[3]; C3DBoundingBox() { } C3DBoundingBox(const C3DBoundingBox& o) { for (int i = 0; i < 3; ++i) { Min[i] = o.Min[i]; Max[i] = o.Max[i]; } }
    void InitialiseFromOBB(const C3DBoundingBox& b, const CMatrix3x4& t) { g_m = &t; g_min0 = b.Min[0]; Max[0] = b.Max[0] * 2; } };
struct SMeshRecord { unsigned char _pad[0x10]; C3DBoundingBox m_localBox; };
struct CMesh { unsigned char _pad[0x50]; SMeshRecord* m_pMesh; unsigned char _pad54[0x1c]; CMatrix3x4 m_transform;
    bool GetBoundingBoxWorldSpace(C3DBoundingBox& box) { C3DBoundingBox local(m_pMesh->m_localBox); box.InitialiseFromOBB(local, m_transform); return true; } };
int main() { SMeshRecord rec; rec.m_localBox.Min[0] = 3; rec.m_localBox.Max[0] = 4; CMesh m; m.m_pMesh = &rec; C3DBoundingBox out; if (!m.GetBoundingBoxWorldSpace(out) || g_m != &m.m_transform || g_min0 != 3 || out.Max[0] != 8) { std::printf("00b9e490_TEST FAIL\n"); return 1; } std::printf("00b9e490_TEST PASS\n"); return 0; }