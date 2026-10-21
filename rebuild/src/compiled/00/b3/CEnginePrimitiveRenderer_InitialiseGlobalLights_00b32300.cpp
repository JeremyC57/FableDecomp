// CEnginePrimitiveRenderer::InitialiseGlobalLights() -- update the lighting manager's global lighting states, then set the two lighting tracked render states from the default @ 0x01396FF4, hand the render core two white colours and enable the tracked state +0x2a48
// Leaf: InitialiseGlobalLights
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B32300, length 416 bytes (body 0x192 + int3 padding), plain `ret` (`this` unused).
// Callees: CEngineLightingManager::UpdateGlobalLightingStates @ 0x00B46C80 (global @ 0x01436E9C); a render-core method taking two
//   `const CRGBColour&` @ 0x00A0AA10 (an empty `ret 8` stub in the PC build; catalogued `$E2`).
// Globals: the render manager core @ 0x01436E18 with its tracked render-state block (capture stack +0x08, dirty list +0x2008,
//   counts +0x2808/+0x280C, capture mask +0x2814, 0x14-byte states; the model of the landed EnableFog 0x00B46890); the states
//   touched are +0x2a70, +0x2a5c (value: the global @ 0x01396FF4) and +0x2a48 (value 1).
// Shape: the manager-side `__forceinline SetState(state, value)` of EnableFog; the colours are CRGBColour temporaries (B,G,R,A
//   bytes, ctor stores R,G,B,A -> +2,+1,+0,+3), the second argument's built first.
#include "engine/CEnginePrimitiveRenderer.h"

struct CRGBColour {                                // B,G,R,A bytes over a packed 32-bit colour
    union {
        struct { unsigned char B, G, R, A; };
        unsigned long Colour;
    };
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

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
    unsigned char        _pad2818[0x2a48 - 0x2818];
    CTrackedRenderState  m_state2a48;              // +0x2A48
    CTrackedRenderState  m_state2a5c;              // +0x2A5C
    CTrackedRenderState  m_state2a70;              // +0x2A70

    void SetGlobalLightColours(const CRGBColour& first, const CRGBColour& second);   // 0x00A0AA10

    __forceinline void SetState(CTrackedRenderState& state, unsigned long value)
    {
        if (state.m_value != value) {
            if ((m_captureMask & state.m_captureMask) == 0) {
                state.m_captureMask |= m_captureMask;
                m_captured[m_capturedCount].m_pState = &state;
                m_captured[m_capturedCount].m_priorValue = state.m_value;
                ++m_capturedCount;
            }
            state.m_value = value;
            if (!state.m_queued) {
                state.m_queued = true;
                m_dirty[m_dirtyCount] = &state;
                ++m_dirtyCount;
            }
        }
    }
};
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18
extern unsigned long g_renderStateDefault_01396ff4;    // 0x01396FF4

struct CEngineLightingManager {
    void UpdateGlobalLightingStates();             // 0x00B46C80
};
extern CEngineLightingManager* g_pLightingManager;   // 0x01436E9C

struct CEnginePrimitiveRenderer_Methods : CEnginePrimitiveRenderer {
    void InitialiseGlobalLights();
};

void CEnginePrimitiveRenderer_Methods::InitialiseGlobalLights()
{
    g_pLightingManager->UpdateGlobalLightingStates();
    g_pRenderManagerCore->SetState(g_pRenderManagerCore->m_state2a70, g_renderStateDefault_01396ff4);
    g_pRenderManagerCore->SetState(g_pRenderManagerCore->m_state2a5c, g_renderStateDefault_01396ff4);
    g_pRenderManagerCore->SetGlobalLightColours(CRGBColour(255, 255, 255, 255), CRGBColour(255, 255, 255, 255));
    g_pRenderManagerCore->SetState(g_pRenderManagerCore->m_state2a48, 1);
}