// CEnginePrimitiveManagerOcclusionBase::DrawDebugVisibilityTestBox(const CMatrix3x4& box, unsigned long colour) -- open a render-state capture block, set thirteen tracked render states (texture-stage colour/alpha ops and args from the globals 0x01396E74/0x01396EF4/0x01396E70, the texture factor = colour, +0x28B8 = 1, +0x291C/+0x2930 from 0x01396F78/0x01396F7C, +0x2840 = 0xF, +0x2868 = 0, +0x282C from 0x01396FA8), render the occlusion data's untextured test mesh with the box, and restore the capture block
// Leaf: DrawDebugVisibilityTestBox
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B90700, length 1488 bytes, `ret 8`.
// Callees: RenderUntexturedStaticMeshPrimitive @ 0x00B903D0 (on `this`: the mesh, the box; as the landed DrawVisibilityTestBox
//   0x00B906C0), CRenderStateManager::RestoreCaptureBlock @ 0x00A05840 (global 0x013CB508; the capture-block open is the inline
//   of the landed 0x00BE1E70 / 0x00B89A20). Render core 0x01436E18: the model of the landed CEnginePrimitiveRenderUtil::
//   SetupStencilMask 0x00BF1030 with the `__forceinline` SetState(state, value, core).
// Signature: Ego_r.pdb `DrawDebugVisibilityTestBox(const CMatrix3x4&, unsigned long)` (the landed caller RenderBoundingBox 0x00B91140).
struct SCaptureEntry { void* Block; unsigned long State; };
struct CCaptureStateManager {
    unsigned char _pad000[8];
    SCaptureEntry CaptureStack[0x500];             // +0x008
    unsigned long CaptureStackCount;               // +0x2808
    unsigned char _pad280c[8];
    unsigned long CaptureMask;                     // +0x2814
    unsigned char _pad2818[0x3a24 - 0x2818];
    unsigned char CaptureBlock[4];                 // +0x3a24
    void OpenCaptureBlock()
    {
        CaptureStack[CaptureStackCount++].Block = CaptureBlock;
        CaptureMask <<= 1;
    }
    void RestoreCaptureBlock();                    // 0x00A05840
};
extern CCaptureStateManager* g_pRenderStateManager;   // 0x013CB508

struct CTrackedRenderState {                       // 0x14 bytes
    long          m_stateId;                       // +0x00
    unsigned long m_value;                         // +0x04 requested value
    unsigned long m_captureMask;                   // +0x08
    long          m_appliedValue;                  // +0x0C
    bool          m_queued;                        // +0x10
};
struct SCapturedRenderState {
    CTrackedRenderState* m_pState;                 // +0x00
    unsigned long        m_priorValue;             // +0x04
};
struct CRenderManagerCoreView {                    // the render manager core @ 0x01436E18
    unsigned char        _pad0000[0x8];
    SCapturedRenderState m_captured[0x400];        // +0x0008
    CTrackedRenderState* m_dirty[0x200];           // +0x2008
    long                 m_capturedCount;          // +0x2808
    long                 m_dirtyCount;             // +0x280C
    long                 m_unknown2810;            // +0x2810
    unsigned long        m_captureMask;            // +0x2814
    unsigned char        _pad2818[0x282c - 0x2818];
    CTrackedRenderState  m_state282c;               // +0x282C
    CTrackedRenderState  m_state2840;               // +0x2840
    unsigned char        _pad2854[0x2868 - 0x2854];
    CTrackedRenderState  m_state2868;               // +0x2868
    unsigned char        _pad287c[0x28b8 - 0x287c];
    CTrackedRenderState  m_state28b8;               // +0x28B8
    unsigned char        _pad28cc[0x291c - 0x28cc];
    CTrackedRenderState  m_state291c;               // +0x291C
    CTrackedRenderState  m_state2930;               // +0x2930
    unsigned char        _pad2944[0x29d0 - 0x2944];
    CTrackedRenderState  m_textureFactor;           // +0x29D0
    unsigned char        _pad29e4[0x3204 - 0x29e4];
    CTrackedRenderState  m_state3204;               // +0x3204
    CTrackedRenderState  m_state3218;               // +0x3218
    unsigned char        _pad322c[0x32a4 - 0x322c];
    CTrackedRenderState  m_state32a4;               // +0x32A4
    CTrackedRenderState  m_state32b8;               // +0x32B8
    unsigned char        _pad32cc[0x3344 - 0x32cc];
    CTrackedRenderState  m_state3344;               // +0x3344
    unsigned char        _pad3358[0x3484 - 0x3358];
    CTrackedRenderState  m_state3484;               // +0x3484
};
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18
extern unsigned long g_renderStateDefault_01396e70;    // 0x01396E70
extern unsigned long g_renderStateDefault_01396e74;    // 0x01396E74
extern unsigned long g_renderStateDefault_01396ef4;    // 0x01396EF4
extern unsigned long g_renderStateDefault_01396f78;    // 0x01396F78
extern unsigned long g_renderStateDefault_01396f7c;    // 0x01396F7C
extern unsigned long g_renderStateDefault_01396fa8;    // 0x01396FA8

static __forceinline void SetState(CTrackedRenderState& state, unsigned long value, CRenderManagerCoreView* core)
{
    if (state.m_value != value) {
        if ((core->m_captureMask & state.m_captureMask) == 0) {
            state.m_captureMask |= core->m_captureMask;
            core->m_captured[core->m_capturedCount].m_pState = &state;
            core->m_captured[core->m_capturedCount].m_priorValue = state.m_value;
            ++core->m_capturedCount;
        }
        state.m_value = value;
        if (!state.m_queued) {
            state.m_queued = true;
            core->m_dirty[core->m_dirtyCount] = &state;
            ++core->m_dirtyCount;
        }
    }
}

class CMatrix3x4 { public: float M[12]; };

struct COcclusionData {
    unsigned char _pad[0x94];
    void*         m_pTestMesh;                     // +0x94
};

struct CEnginePrimitiveManagerOcclusionBase {
    unsigned char   _pad[0x14];
    COcclusionData* m_pOcclusionData;              // +0x14
    void RenderUntexturedStaticMeshPrimitive(void* mesh, const CMatrix3x4& box);   // 0x00B903D0
    void DrawDebugVisibilityTestBox(const CMatrix3x4& box, unsigned long colour);
};

void CEnginePrimitiveManagerOcclusionBase::DrawDebugVisibilityTestBox(const CMatrix3x4& box, unsigned long colour)
{
    g_pRenderStateManager->OpenCaptureBlock();
    SetState(g_pRenderManagerCore->m_state3204, g_renderStateDefault_01396e74, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3344, g_renderStateDefault_01396ef4, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state32a4, g_renderStateDefault_01396e74, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3484, g_renderStateDefault_01396ef4, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3218, g_renderStateDefault_01396e70, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state32b8, g_renderStateDefault_01396e70, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_textureFactor, colour, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state28b8, 1, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state291c, g_renderStateDefault_01396f78, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2930, g_renderStateDefault_01396f7c, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2840, 0xf, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2868, 0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state282c, g_renderStateDefault_01396fa8, g_pRenderManagerCore);
    RenderUntexturedStaticMeshPrimitive(m_pOcclusionData->m_pTestMesh, box);
    g_pRenderStateManager->RestoreCaptureBlock();
}