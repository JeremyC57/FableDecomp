#include <cstdio>
#include <cstddef>
// Behaviour model of CRenderTarget::CRenderTarget @ 0x009F9CD0.
namespace N_0099a2d0 { struct CBase { virtual void v00() {} CBase() {} }; } using N_0099a2d0::CBase;
class CSurface { public: virtual void v00() {} void* m_pSurface; void* m_pTexture; long m_format; CSurface() : m_pSurface(0), m_pTexture(0) {} };
class CRenderTarget : public CBase { public: CSurface m_colour, m_depth; virtual void v00() {} CRenderTarget() : CBase(), m_colour(), m_depth() {} };
int main()
{
    CRenderTarget r;
    bool ok = r.m_colour.m_pSurface == 0 && r.m_colour.m_pTexture == 0 && r.m_depth.m_pSurface == 0 && r.m_depth.m_pTexture == 0;
    if (sizeof(void*) == 4) ok = ok && offsetof(CRenderTarget, m_colour) == 0x4 && offsetof(CRenderTarget, m_depth) == 0x14;
    if (!ok) { std::printf("009f9cd0_TEST FAIL\n"); return 1; }
    std::printf("009f9cd0_TEST PASS\n");
    return 0;
}