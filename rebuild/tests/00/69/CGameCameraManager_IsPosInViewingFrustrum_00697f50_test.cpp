#include <cstdio>
#include <cstdlib>
#include <cstring>
class C3DVector { public: float X, Y, Z; }; static const void* g_pos = 0; static const void* g_matrix = 0; static const void* g_planes = 0; static float g_fov = 0; static bool g_result = false;
struct CCameraObject_00697f50 { unsigned char _body[0x74]; float GetFieldOfView() const { return 1.25f; } };
struct SViewRecord_00697f50 { unsigned char _pad[0x10]; unsigned char m_matrix[0x44]; unsigned char m_planes[0x40]; };
struct CTools_00697f50 { unsigned char _body[0x20]; bool IsPosInViewingFrustrum(const C3DVector& p, const void* m, const void* pl, float f) { g_pos = &p; g_matrix = m; g_planes = pl; g_fov = f; return g_result; } };
class CGameCameraManager { public: virtual void v00() { } CCameraObject_00697f50 m_camera; CTools_00697f50 m_tools; SViewRecord_00697f50* m_pView; bool Test(const C3DVector& p) const { return IsPosInViewingFrustrum(p); }
    virtual bool IsPosInViewingFrustrum(const C3DVector& pos) const { return const_cast<CTools_00697f50&>(m_tools).IsPosInViewingFrustrum(pos, m_pView->m_matrix, m_pView->m_planes, m_camera.GetFieldOfView()); } };
int main() {
    CGameCameraManager m; SViewRecord_00697f50 v; m.m_pView = &v; C3DVector p; g_result = true; if (!m.Test(p) || g_pos != &p || g_matrix != v.m_matrix || g_planes != v.m_planes || g_fov != 1.25f) { std::printf("00697f50_TEST FAIL\n"); return 1; }
    g_result = false; if (m.Test(p)) { std::printf("00697f50_TEST FAIL\n"); return 1; }
    std::printf("00697f50_TEST PASS\n"); return 0;
}