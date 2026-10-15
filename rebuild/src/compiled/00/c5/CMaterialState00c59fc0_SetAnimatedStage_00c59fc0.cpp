// CMaterialState_00c59fc0::SetAnimatedStage(SStageDesc* desc) -- applies the stage's animation transform, then (for a non-null descriptor) converts its first word with the global time/pixel converter into +0x48
// Leaf: SetAnimatedStage
// Retail: Fable.exe @ 0x00C59FC0, 37 bytes (`ret 4`).
// Callees: CMaterialState::ApplyStageTransform 0x00C59C60 (thiscall; manifest
//          "GraphicalElement_ComputeAnimationTransform"), the __cdecl converter 0x00C59B00 (manifest
//          "Graphics_ConvertTimeToPixels"; reads the globals 0x0139CA14/0x0139C9F0).
// Label: the manifest's "CTCTavernGameCoinGolf::DrawPreparePrimitivesForRendering" is propagated. The
// only caller is the material set-up at 0x00C5BD00 (no-RTTI render module: it ORs stage flags and
// fills +0x74/+0x7C/+0x84), so the class is named by address and the members are descriptive.
// Shape: the argument is cached in edi across the first call; `add esp,4` after the __cdecl call
//        means its result is used (stored).
struct SStageDesc {
    long m_frames;                      // +0x00 (converted by 0x00C59B00)
};

long __cdecl ConvertStageFrames(long frames);   // 0x00C59B00

struct CMaterialState_00c59fc0 {
    unsigned char _pad[0x48];
    long m_stagePixels;                 // +0x48
    void ApplyStageTransform(SStageDesc* desc);  // 0x00C59C60
    void SetAnimatedStage(SStageDesc* desc);
};

void CMaterialState_00c59fc0::SetAnimatedStage(SStageDesc* desc)
{
    ApplyStageTransform(desc);
    if (desc)
        m_stagePixels = ConvertStageFrames(desc->m_frames);
}