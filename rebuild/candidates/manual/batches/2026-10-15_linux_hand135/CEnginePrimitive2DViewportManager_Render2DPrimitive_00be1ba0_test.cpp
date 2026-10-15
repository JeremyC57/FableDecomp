// Behaviour test for CEnginePrimitive2DViewportManager::Render2DPrimitive @ 0x00BE1BA0.
#include <cstdio>
struct CBase { virtual int Kind() { return 0; } virtual ~CBase() { } };
struct CEngineInternalPrimitiveBase { long m_layer; };
struct CEngineSubPrimitiveChain { int id; };
struct CEnginePrimitive2DViewportManager;
struct CEngineSubPrimitiveBase : CBase {
    CEngineInternalPrimitiveBase* m_pPrimitive; CEngineSubPrimitiveBase* m_pNext;
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase& p) : m_pPrimitive(&p), m_pNext(0) { }
};
struct CEngineSubPrimitive2DViewport : CEngineSubPrimitiveBase {
    virtual int Kind() { return 7; }
    CEngineSubPrimitive2DViewport(CEngineInternalPrimitiveBase& p) : CEngineSubPrimitiveBase(p) { }
};
static long g_layerSeen = -1;
struct CEnginePrimitiveRenderer2D { float Layer2DToDistance(long layer) { g_layerSeen = layer; return layer * 10.0f; } };
static CEnginePrimitiveRenderer2D g_r2d; static CEnginePrimitiveRenderer2D* g_pR2D = &g_r2d;
static CEngineSubPrimitiveChain* g_chain; static CEngineSubPrimitive2DViewport* g_sub; static float g_dist; static CEnginePrimitive2DViewportManager* g_mgr;
struct CEnginePrimitiveRenderer {
    void AddSubPrimitive(CEngineSubPrimitiveChain& c, CEngineSubPrimitive2DViewport* s, float d, CEnginePrimitive2DViewportManager* m)
    { g_chain = &c; g_sub = s; g_dist = d; g_mgr = m; }
};
static CEnginePrimitiveRenderer g_r; static CEnginePrimitiveRenderer* g_pR = &g_r;
struct CEnginePrimitive2DViewportManager {
    void Render2DPrimitive(CEngineInternalPrimitiveBase& primitive, long, CEngineSubPrimitiveChain& chain)
    {
        CEngineSubPrimitive2DViewport* sub = new CEngineSubPrimitive2DViewport(primitive);
        g_pR->AddSubPrimitive(chain, sub, g_pR2D->Layer2DToDistance(primitive.m_layer), this);
    }
};
int main()
{
    CEnginePrimitive2DViewportManager m; CEngineInternalPrimitiveBase p = { 3 }; CEngineSubPrimitiveChain c = { 1 };
    m.Render2DPrimitive(p, 0, c);
    int ok = g_layerSeen == 3 && g_dist == 30.0f && g_chain == &c && g_mgr == &m && g_sub && g_sub->Kind() == 7
          && g_sub->m_pPrimitive == &p && g_sub->m_pNext == 0;
    delete g_sub;
    if (!ok) { std::printf("00be1ba0_TEST FAIL\n"); return 1; }
    std::printf("00be1ba0_TEST PASS\n");
    return 0;
}
