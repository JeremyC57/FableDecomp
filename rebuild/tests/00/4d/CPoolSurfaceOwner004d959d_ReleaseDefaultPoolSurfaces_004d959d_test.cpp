#include <cstdio>
static int g_dtor = 0, g_release = 0; static long g_releaseFlag = -1;
struct COwner { ~COwner() { ++g_dtor; } };
struct CSurface { virtual void v00(){} virtual void v01(){} virtual void v02(){} virtual void v03(){} virtual void v04(){} virtual void v05(){} virtual void v06(){} virtual void v07(){} virtual void v08(){} virtual void v09(){} virtual void v10(){} virtual void v11(){} virtual void v12(){} virtual void v13(){} virtual void v14(){} virtual void v15(){} virtual void v16(){} virtual void v17(){} virtual void v18(){} virtual void v19(){} virtual void v20(){} virtual void v21(){} virtual void v22(){} virtual void Release(long f) { ++g_release; g_releaseFlag = f; } };
struct CPool { unsigned char _pad[0xc]; COwner* m_pOwner; CSurface* m_pSurface;
    void ReleaseDefaultPoolSurfaces() { if (m_pOwner) { delete m_pOwner; if (m_pSurface) m_pSurface->Release(1); m_pOwner = 0; m_pSurface = 0; } } };
int main()
{
    CSurface s; CPool p; p.m_pOwner = 0; p.m_pSurface = &s; p.ReleaseDefaultPoolSurfaces();
    if (g_dtor != 0 || g_release != 0 || p.m_pSurface != &s) { std::printf("004d959d_TEST FAIL\n"); return 1; }
    p.m_pOwner = new COwner; p.ReleaseDefaultPoolSurfaces();
    if (g_dtor != 1 || g_release != 1 || g_releaseFlag != 1 || p.m_pOwner != 0 || p.m_pSurface != 0) { std::printf("004d959d_TEST FAIL\n"); return 1; }
    p.m_pOwner = new COwner; p.ReleaseDefaultPoolSurfaces();
    if (g_dtor != 2 || g_release != 1 || p.m_pOwner != 0) { std::printf("004d959d_TEST FAIL\n"); return 1; }
    std::printf("004d959d_TEST PASS\n"); return 0;
}