// CEngine::GetPrimitiveDrawDistanceFactor() -- (the render settings' distance bias at +0x50 + 1) times the view record's draw-distance scale at +0x7c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23D00, length 24 bytes.
// Callees: none. Globals: the render settings @ 0x01436E20 and the view record @ 0x01436E80 (engine singletons). Shape: `fld [a] ; fadd 1.0 ; fmul [b]` is
// `(a + 1.0f) * b`, the result left in st(0); a static member (no `this`).
struct SRenderSettings_01436e20 {
    unsigned char _pad[0x50];
    float m_distanceBias;                          // +0x50
};
struct SViewRecord_01436e80 {
    unsigned char _pad[0x7c];
    float m_drawDistanceScale;                     // +0x7c
};
extern SRenderSettings_01436e20* g_pRenderSettings_01436e20;   // 0x01436E20
class CEnginePrimitiveRenderer;
extern CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer;   // 0x01436E80 (the engine's primitive-renderer component; Codex name fix 2026-11-10)
// The members read here are modelled by the local SViewRecord_01436e80 view of that object (name debt: the shared
// engine/CEnginePrimitiveRenderer.h retail branch).

class CEngine { public:
    static float GetPrimitiveDrawDistanceFactor();
};

float CEngine::GetPrimitiveDrawDistanceFactor()
{
    return (g_pRenderSettings_01436e20->m_distanceBias + 1.0f) * ((SViewRecord_01436e80*)g_pEnginePrimitiveRenderer)->m_drawDistanceScale;
}