#include <cstdio>
class CMatrix3x4 { public: float m_rows[12]; };
static const CMatrix3x4* g_target = 0; static float g_min0 = 0;
class C3DBoundingBox { public: float Min[3]; float Max[3]; void CalculateMatrix(CMatrix3x4* m) const { g_target = m; g_min0 = Min[0]; } };
struct CGroup {
    bool has;
    virtual bool GetBoundingBoxWorldSpace(C3DBoundingBox& box) { box.Min[0] = 7.0f; return has; }
    bool GetBoundingBoxTransform(CMatrix3x4& t) { C3DBoundingBox box; if (GetBoundingBoxWorldSpace(box)) { box.CalculateMatrix(&t); return true; } return false; }
};
int main()
{
    CGroup g; CMatrix3x4 m; g.has = false;
    if (g.GetBoundingBoxTransform(m) || g_target != 0) { std::printf("00bacbd0_TEST FAIL\n"); return 1; }
    g.has = true;
    if (!g.GetBoundingBoxTransform(m) || g_target != &m || g_min0 != 7.0f) { std::printf("00bacbd0_TEST FAIL\n"); return 1; }
    std::printf("00bacbd0_TEST PASS\n"); return 0;
}