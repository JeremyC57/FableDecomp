#include <cstdio>
struct CMatrix3x4 { float m_rows[12]; }; static const CMatrix3x4* g_m = 0; static float g_min0 = -1;
struct C3DBoundingBox { float Min[3]; float Max[3]; C3DBoundingBox() { } C3DBoundingBox(const C3DBoundingBox& o) { for (int i = 0; i < 3; ++i) { Min[i] = o.Min[i]; Max[i] = o.Max[i]; } }
    void InitialiseFromOBB(const C3DBoundingBox& b, const CMatrix3x4& t) { g_m = &t; g_min0 = b.Min[0]; Max[0] = b.Max[0] * 2; } };
struct CMeshInfo { unsigned char _pad[0x10]; C3DBoundingBox m_boundingBox; };
struct CMesh { unsigned char _pad[0x50]; CMeshInfo* m_pMeshInfo; unsigned char _pad54[0x1c]; CMatrix3x4 ObjectTransform;
    bool GetBoundingBoxWorldSpace(C3DBoundingBox& box) { C3DBoundingBox local(m_pMeshInfo->m_boundingBox); box.InitialiseFromOBB(local, ObjectTransform); return true; } };
int main() { CMeshInfo info; info.m_boundingBox.Min[0] = 3; info.m_boundingBox.Max[0] = 4; CMesh m; m.m_pMeshInfo = &info; C3DBoundingBox out; if (!m.GetBoundingBoxWorldSpace(out) || g_m != &m.ObjectTransform || g_min0 != 3 || out.Max[0] != 8) { std::printf("00b9db80_TEST FAIL\n"); return 1; } std::printf("00b9db80_TEST PASS\n"); return 0; }