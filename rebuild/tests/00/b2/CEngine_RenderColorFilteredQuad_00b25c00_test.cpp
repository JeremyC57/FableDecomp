// Behaviour test for CEngine::RenderColorFilteredQuad @ 0x00B25C00
#include <cstdio>
class CTexture { public: int id; };
class C4DVector { public: float X, Y, Z, W; };
struct CStateInfo { int dummy; };
struct CStateStackElement { CStateInfo* State; unsigned int Value; };
static int g_log[16]; static int g_logCount = 0;
class CRenderStateManager { public:
    CStateStackElement StateStack[16];
    long StateStackSize;
    unsigned int CurrentBookmarkMask;
    CStateInfo BookmarkDummyState;
    void CreateCaptureBlock() { long& size = StateStackSize; StateStack[size].State = &BookmarkDummyState; ++size; CurrentBookmarkMask <<= 1; }
    void RestoreCaptureBlock() { --StateStackSize; CurrentBookmarkMask >>= 1; g_log[g_logCount++] = 9; }
};
static CRenderStateManager g_manager;
CRenderStateManager* g_pRenderStateManager = &g_manager;
class CShaderRenderManager { public:
    long m_vertexShaderConstantLayout;
    void SetVertexShaderConstantLayout(long layout) { m_vertexShaderConstantLayout = layout; g_log[g_logCount++] = (int)layout; }
};
CShaderRenderManager g_ShaderRenderManager;
class CEngineScreenEffectColourFilter { public:
    float m_x; const C4DVector* m_colour; CTexture* m_texture; long m_layoutSeen;
    void RenderColorFilteredQuad(float x, float, const C4DVector& colour, float, float, CTexture* texture)
    { m_x = x; m_colour = &colour; m_texture = texture; m_layoutSeen = g_ShaderRenderManager.m_vertexShaderConstantLayout; g_log[g_logCount++] = 7; }
};
static CEngineScreenEffectColourFilter g_filter;
CEngineScreenEffectColourFilter* g_pScreenEffectColourFilter = &g_filter;
class CEngine { public:
    int m_defaults;
    void SetDefaultRenderStates() { ++m_defaults; g_log[g_logCount++] = 1; }
    void RenderColorFilteredQuad(float x, float y, const C4DVector& colour, float width, float height, CTexture* texture)
    {
        g_pRenderStateManager->CreateCaptureBlock();
        SetDefaultRenderStates();
        long layout = g_ShaderRenderManager.m_vertexShaderConstantLayout;
        g_ShaderRenderManager.SetVertexShaderConstantLayout(3);
        g_pScreenEffectColourFilter->RenderColorFilteredQuad(x, y, colour, width, height, texture);
        g_ShaderRenderManager.SetVertexShaderConstantLayout(layout);
        g_pRenderStateManager->RestoreCaptureBlock();
    }
};
int main()
{
    g_manager.StateStackSize = 2; g_manager.CurrentBookmarkMask = 1;
    g_ShaderRenderManager.m_vertexShaderConstantLayout = 5;
    CEngine e; e.m_defaults = 0;
    C4DVector c = { 1, 0, 0, 1 }; CTexture t = { 4 };
    e.RenderColorFilteredQuad(2.5f, 3.5f, c, 10, 20, &t);
    if (g_manager.StateStack[2].State != &g_manager.BookmarkDummyState || g_manager.StateStackSize != 2 || g_manager.CurrentBookmarkMask != 1 ||
        g_filter.m_x != 2.5f || g_filter.m_colour != &c || g_filter.m_texture != &t || g_filter.m_layoutSeen != 3 ||
        g_ShaderRenderManager.m_vertexShaderConstantLayout != 5 || g_logCount != 5 || g_log[0] != 1 || g_log[1] != 3 || g_log[2] != 7 ||
        g_log[3] != 5 || g_log[4] != 9) {
        std::printf("00b25c00_TEST FAIL\n"); return 1;
    }
    std::printf("00b25c00_TEST PASS\n");
    return 0;
}