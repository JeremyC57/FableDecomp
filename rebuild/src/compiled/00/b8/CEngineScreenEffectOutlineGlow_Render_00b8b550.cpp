// Leaf: ?Render@CEngineScreenEffectOutlineGlow@@UAEXABVCEngineComponentRenderParams@@@Z
// CEngineScreenEffectOutlineGlow::Render(const CEngineComponentRenderParams& params) -- when enabled (+8) and the params' kind (+4) is the outline pass (0x4000000): apply the outline, then release the textures
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8B550, length 39 bytes.
// Callees: ApplyOutline @ 0x00B89C30 and the texture release @ 0x00B89080 (both on `this`). Shape: the flag byte through al, the immediate compare on the params word,
// `ret 4`.
class CEngineComponentRenderParams { public:
    long m_a;
    long m_kind;                                   // +0x04
};

class CEngineScreenEffectOutlineGlow { public:
    unsigned char _pad[0x4];
    bool          m_enabled;                       // +0x08
    private: void ApplyOutline(); public:                           // 0x00B89C30
private:
    void FinishWithDynamicTextures();              // 0x00B89080 (NF35: Ego_r + FableWin ?FinishWithDynamicTextures@CEngineScreenEffectOutlineGlow@@AAEXXZ)
public:
    virtual void Render(const CEngineComponentRenderParams& params);
};

void CEngineScreenEffectOutlineGlow::Render(const CEngineComponentRenderParams& params)
{
    if (m_enabled && params.m_kind == 0x4000000) {
        ApplyOutline();
        FinishWithDynamicTextures();
    }
}