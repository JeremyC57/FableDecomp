// CEngine::RenderColorFilteredQuad(float x, float y, const C4DVector& colour, float width, float height, CTexture* texture) -- draw one colour-filtered screen quad inside a render-state capture block, with the default render states and vertex-shader constant layout 3, restoring the previous layout afterwards
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B25C00, length 124 bytes (+ int3 padding).
// Callees: CEngine::SetDefaultRenderStates() @ 0x00B24850 (on `this`, which is still in ecx), CShaderRenderManager::SetVertexShaderConstantLayout(long)
// @ 0x0098B5E0 (on the global object @ 0x013BC470, current layout at +8), CEngineScreenEffectColourFilter::RenderColorFilteredQuad(...) @ 0x00B5AF80
// (on the object behind the global @ 0x01436E40, the six arguments forwarded), CRenderStateManager::RestoreCaptureBlock() @ 0x00A05840 (on the
// global @ 0x013CB508). Argument names are descriptive (manifest prototype: float, float, const C4DVector&, float, float, CTexture*).
// Layout: PDB CRenderStateManager: StateStack (CStateStackElement[1024], 8 bytes each) +0x08, StateStackSize +0x2808, CurrentBookmarkMask
// +0x2814, BookmarkDummyState +0x3a24.
// Shape: the inlined capture-block push (`lea edx,[eax+2808h]` then `inc dword ptr [edx]`: the stack size through a reference, the dummy
// state stored into the next stack slot, the bookmark mask shifted left); the current layout read into esi before the `SetVertex...(3)`
// call; the six arguments pushed right to left; the render-state manager pointer re-read for the restore.
class CTexture;

class C4DVector { public:
    float X, Y, Z, W;
};

struct CStateInfo {
    unsigned char _body[0x14];
};

struct CStateStackElement {
    CStateInfo*   State;
    unsigned long Value;
};

class CRenderStateManager { public:
    void*              __vftable;            // +0x00
    void*              PD3DDevice;           // +0x04
    CStateStackElement StateStack[1024];     // +0x08
    CStateInfo*        StateUpdateList[256]; // +0x2008
    CStateInfo*        AllStatesList[256];   // +0x2408
    long               StateStackSize;       // +0x2808
    long               StateUpdateListSize;  // +0x280c
    long               AllStatesListSize;    // +0x2810
    unsigned long      CurrentBookmarkMask;  // +0x2814
    unsigned char      DeviceState[0x3a24 - 0x2818];   // +0x2818 CDeviceState
    CStateInfo         BookmarkDummyState;   // +0x3a24
    void CreateCaptureBlock()
    {
        long& size = StateStackSize;
        StateStack[size].State = &BookmarkDummyState;
        ++size;
        CurrentBookmarkMask <<= 1;
    }
    void RestoreCaptureBlock();              // 0x00A05840
};
extern CRenderStateManager* g_pRenderStateManager;   // 0x013CB508

class CShaderRenderManager { public:
    unsigned char _pad00[0x08];
    long          m_vertexShaderConstantLayout;      // +0x08
    void SetVertexShaderConstantLayout(long layout);  // 0x0098B5E0
};
extern CShaderRenderManager g_ShaderRenderManager;   // 0x013BC470

class CEngineScreenEffectColourFilter { public:
    void RenderColorFilteredQuad(float x, float y, const C4DVector& colour, float width, float height, CTexture* texture);   // 0x00B5AF80
};
extern CEngineScreenEffectColourFilter* g_pScreenEffectColourFilter;   // 0x01436E40

class CEngine { public:
    void SetDefaultRenderStates();           // 0x00B24850
    void RenderColorFilteredQuad(float x, float y, const C4DVector& colour, float width, float height, CTexture* texture);
};

void CEngine::RenderColorFilteredQuad(float x, float y, const C4DVector& colour, float width, float height, CTexture* texture)
{
    g_pRenderStateManager->CreateCaptureBlock();
    SetDefaultRenderStates();
    long layout = g_ShaderRenderManager.m_vertexShaderConstantLayout;
    g_ShaderRenderManager.SetVertexShaderConstantLayout(3);
    g_pScreenEffectColourFilter->RenderColorFilteredQuad(x, y, colour, width, height, texture);
    g_ShaderRenderManager.SetVertexShaderConstantLayout(layout);
    g_pRenderStateManager->RestoreCaptureBlock();
}