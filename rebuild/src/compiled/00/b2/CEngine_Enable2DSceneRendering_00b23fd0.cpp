// CEngine::Enable2DSceneRendering(bool) -- clear bit 2 of the scene-rendering flag word at +0xb8, then set it again when enabling
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23FD0, length 33 bytes.
// Callees: none. Shape: `flags &= ~2` is stored before the branch and `flags |= 2` stored again on the enable path (the register copy is reused). The
// retail flag word sits at +0xb8 (the landed Is3DSceneRenderingEnabled @ 0x00B23FC0 reads bit 0 there); Ego_r.pdb's CEngine has WorldFramesPerSecond at
// that offset, so engine/CEngine.h cannot be included and the word is modelled locally.
class CEngine { public:
    unsigned char _pad[0xb8];
    unsigned long m_sceneRenderFlags;              // +0xb8: bit 0 = 3D scene, bit 1 = 2D scene
    void Enable2DSceneRendering(bool enable);
};

void CEngine::Enable2DSceneRendering(bool enable)
{
    m_sceneRenderFlags &= ~2;
    if (enable)
        m_sceneRenderFlags |= 2;
}