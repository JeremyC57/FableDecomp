// CFalloffTable_00c458a0::ApplyFalloff(CFalloffCurve& curve) -- replace the first float of each of the eight 0x1c-byte entries at +0x18 with the curve's falloff of it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C458A0, length 38 bytes.
// Callees: the curve's Calculate @ 0x00C1BDA0 (on the curve, the float by value, a float result in st0: the manifest label "Render_CalculateAlphaFalloff").
// Shape: a counted loop (`mov edi,8 ; ... ; dec edi ; jne`) over the entry pointer in esi (`add esi,1Ch`), the result stored with `fstp dword ptr [esi]`; the
// loop head is aligned with `nop`. The chunk label "CEngineResourceManager::UnloadShaderCode" is propagated; the class is named by address.
struct CFalloffCurve_00c1bda0 {
    float Calculate(float value);                  // 0x00C1BDA0
};

struct SFalloffEntry { float m_value; unsigned char _rest[0x18]; };   // 0x1c bytes

struct CFalloffTable_00c458a0 {
    unsigned char _pad[0x18];
    SFalloffEntry m_entries[8];                    // +0x18
    void ApplyFalloff(CFalloffCurve_00c1bda0& curve);
};

void CFalloffTable_00c458a0::ApplyFalloff(CFalloffCurve_00c1bda0& curve)
{
    for (long i = 0; i < 8; ++i)
        m_entries[i].m_value = curve.Calculate(m_entries[i].m_value);
}