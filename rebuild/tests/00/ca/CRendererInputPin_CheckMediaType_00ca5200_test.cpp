// Test for CRendererInputPin::CheckMediaType (0x00CA5200): the pin asks its owning renderer through the renderer's
// virtual CheckMediaType and returns the renderer's HRESULT unchanged.
#include <cstdio>
typedef long HRESULT;
struct CMediaType { int subtype; };
static const CMediaType* g_seen = 0;
struct CBaseRenderer {
    virtual ~CBaseRenderer() {}
    virtual HRESULT CheckMediaType(const CMediaType* pmt) = 0;
};
struct CRgbRenderer : CBaseRenderer {
    HRESULT CheckMediaType(const CMediaType* pmt) { g_seen = pmt; return pmt->subtype == 24 ? 0 : (HRESULT)0x80040200L; }
};
struct CRendererInputPin {
    CBaseRenderer* m_pRenderer;
    HRESULT CheckMediaType(const CMediaType* pmt) { return m_pRenderer->CheckMediaType(pmt); }
};
int main()
{
    CRgbRenderer r; CRendererInputPin pin; pin.m_pRenderer = &r;
    CMediaType good = { 24 }, bad = { 16 };
    if (pin.CheckMediaType(&good) != 0 || g_seen != &good) { std::printf("00ca5200_TEST FAIL\n"); return 1; }
    if (pin.CheckMediaType(&bad) != (HRESULT)0x80040200L || g_seen != &bad) { std::printf("00ca5200_TEST FAIL\n"); return 1; }
    std::printf("00ca5200_TEST PASS\n");
    return 0;
}