// CEnvironmentSpellEffectInterface::GetRenderFrameInterpolatedScreenFilterParameters(SFilterParams* result, void* view) -- transform the filter record at +0x58 for the view into a 12-byte local and copy it out (the weight through the x87, the two words as integers)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075CCB0, length 50 bytes.
// Callees: TransformVector @ 0x0075C280 (on `this`: the local by address, the member by address, the view; the manifest label "CTCCameraPointDefinitionBase::TransformVector"
// is propagated). Shape: a 12-byte frame, the member address formed with `lea edx`, the result pointer re-read from the stack after the call; `ret 8`.
struct SFilterParams_0075ccb0 {                     // 12 bytes: a float weight and two words
    float m_weight;
    long  m_a;
    long  m_b;
};

struct CEnvironmentSpellEffectInterface {
    unsigned char       _pad[0x58];
    SFilterParams_0075ccb0 m_filter;                  // +0x58
    void TransformVector(SFilterParams_0075ccb0& out, const SFilterParams_0075ccb0& in, void* view);   // 0x0075C280
    void GetRenderFrameInterpolatedScreenFilterParameters(SFilterParams_0075ccb0* result, void* view);
};

void CEnvironmentSpellEffectInterface::GetRenderFrameInterpolatedScreenFilterParameters(SFilterParams_0075ccb0* result, void* view)
{
    SFilterParams_0075ccb0 params;
    TransformVector(params, m_filter, view);
    result->m_weight = params.m_weight;
    result->m_a = params.m_a;
    result->m_b = params.m_b;
}