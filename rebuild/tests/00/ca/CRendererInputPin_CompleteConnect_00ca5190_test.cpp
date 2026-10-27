#include <cstdio>
// CRendererInputPin::CompleteConnect: the renderer first; its failure is returned; else the base pin's result.
struct Renderer { int verdict; int calls; int CompleteConnect(void* pin) { ++calls; return verdict; } };
static int g_baseCalls = 0;
struct BasePin { int CompleteConnect(void* pin) { ++g_baseCalls; return 0; } };
struct RendererInputPin : BasePin {
    Renderer* m_pRenderer;
    int CompleteConnect(void* pReceivePin)
    {
        int hr = m_pRenderer->CompleteConnect(pReceivePin);
        if (hr < 0) return hr;
        return BasePin::CompleteConnect(pReceivePin);
    }
};
int main()
{
    Renderer r; r.verdict = (int)0x80004005; r.calls = 0;
    RendererInputPin p; p.m_pRenderer = &r;
    if (p.CompleteConnect(&r) != (int)0x80004005 || g_baseCalls != 0 || r.calls != 1) { std::printf("00ca5190_TEST FAIL\n"); return 1; }
    r.verdict = 1;
    if (p.CompleteConnect(&r) != 0 || g_baseCalls != 1) { std::printf("00ca5190_TEST FAIL\n"); return 1; }
    std::printf("00ca5190_TEST PASS\n");
    return 0;
}