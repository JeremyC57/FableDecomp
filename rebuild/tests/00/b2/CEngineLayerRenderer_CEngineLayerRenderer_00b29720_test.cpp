#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
static int g_base = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct __declspec(novtable) CEngineLayerRendererBase : CBase { void* m_pScene; CEngineLayerRendererBase(void* s) : CBase(), m_pScene(s) { } };
struct CRenderItemVector { void* m_pFirst; void* m_pLast; void* m_pEnd; CRenderItemVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { } };
class CEngineLayerRenderer : public CEngineLayerRendererBase { public:
    void* m_pLayer; void* m_pTarget; CRenderItemVector m_items; virtual void v00() { }
    CEngineLayerRenderer(void* s, void* l, void* t) : CEngineLayerRendererBase(s), m_pLayer(l), m_pTarget(t), m_items() { }
};
int main()
{
    int a, b, c; CEngineLayerRenderer v(&a, &b, &c);
    if (g_base != 1 || v.m_pScene != &a || v.m_pLayer != &b || v.m_pTarget != &c || v.m_items.m_pFirst || v.m_items.m_pLast || v.m_items.m_pEnd) { std::printf("00b29720_TEST FAIL\n"); return 1; }
    std::printf("00b29720_TEST PASS\n"); return 0;
}