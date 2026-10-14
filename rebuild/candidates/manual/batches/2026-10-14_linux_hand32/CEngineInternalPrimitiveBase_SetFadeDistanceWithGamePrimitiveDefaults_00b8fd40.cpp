// CEngineInternalPrimitiveBase::SetFadeDistanceWithGamePrimitiveDefaults(float distance) -- a non-positive distance is replaced by the render settings' default fade distance; the fade-begin distance is stored and the max draw distance derived with the settings' bias
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8FD40, length 59 bytes.
// Callees: none. Globals: the render settings @ 0x01436E20 (default fade distance +0x4c, distance bias +0x50). Constants 0.0 (0x0122DEDC), 1.0 (0x0122DED8).
// Shape: `fcomp 0.0 ; test ah,41h ; jp` is the `d <= 0` test (even parity = `d > 0` skips the replacement) (the replacement is written back into the argument slot), then the two stores; the max draw
// distance is `(bias + 1.0f) * distance`. engine/CEngineInternalPrimitiveBase.h (PDB): FadeDistance_FadeBeginDistance +0x14, FadeDistance_MaxDrawDistance +0x18.
struct SRenderSettings_01436e20 {
    unsigned char _pad[0x4c];
    float m_defaultFadeDistance;                   // +0x4c
    float m_drawDistanceBias;                      // +0x50
};
extern SRenderSettings_01436e20* g_pRenderSettings_01436e20;   // 0x01436E20

struct CEngineInternalPrimitiveBase {
    unsigned char _pad[0x14];
    float FadeDistance_FadeBeginDistance;          // +0x14
    float FadeDistance_MaxDrawDistance;            // +0x18
    void SetFadeDistanceWithGamePrimitiveDefaults(float distance);
};

void CEngineInternalPrimitiveBase::SetFadeDistanceWithGamePrimitiveDefaults(float distance)
{
    if (distance <= 0.0f)                          // `fcomp ; test ah,41h ; jp` (jump when neither C0 nor C3: d > 0)
        distance = g_pRenderSettings_01436e20->m_defaultFadeDistance;
    FadeDistance_FadeBeginDistance = distance;
    FadeDistance_MaxDrawDistance = (g_pRenderSettings_01436e20->m_drawDistanceBias + 1.0f) * distance;
}
