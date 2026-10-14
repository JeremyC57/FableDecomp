// CSubtitleRenderer::ResetCutsceneState() -- leave cutscene mode and reset the four display flags.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004933E6, length 40 bytes.
// Callee: CSubtitleRenderer::SetAsInCutscene(bool) @ 0x0049319C with `false` (the manifest labels the callee
// CSubtitleRenderer and this body "CAIStateGroup_MinionBehaviourSad::CleanupLeaf", a propagated AI label --
// the +0xe0.. flag bytes and the subtitle callee place it in the renderer; module/leaf are descriptive).
// Shape: the call first (`push 0` before `mov esi,ecx` is scheduling), then one `true` and three `false`
// byte stores at +0xe0, +0xe1, +0xe2, +0xe6 in statement order.
struct CSubtitleRenderer {
    unsigned char _pad[0xe0];
    bool m_visible;                         // +0xe0
    bool m_fading;                          // +0xe1
    bool m_queued;                          // +0xe2
    unsigned char _pade3[3];                // +0xe3
    bool m_forced;                          // +0xe6
    void SetAsInCutscene(bool inCutscene);  // 0x0049319C
    void ResetCutsceneState();
};

void CSubtitleRenderer::ResetCutsceneState()
{
    SetAsInCutscene(false);
    m_visible = true;
    m_fading = false;
    m_queued = false;
    m_forced = false;
}