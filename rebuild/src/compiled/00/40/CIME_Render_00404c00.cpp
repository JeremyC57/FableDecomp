// CIME::Render() -- when the IME is active (+8): publish the alpha byte (+3) of the current theme record (the object at +0x4c, virtual slot 27) into the global at 0x013750B0 (zero without an object), render the composition string (0x00CB38E0, `__fastcall`, both flags set), and draw the front-end manager's sprite at +0xb8 when it exists (CSprite::DrawWithRenderManager @ 0x0041A980, a tail jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00404C00, length 80 bytes.
// Shape: `mov dl,1 ; mov cl,dl`, the global byte store, `jmp` for the last call.
extern unsigned char g_imeCompositionAlpha_013750b0;   // 0x013750B0
struct SThemeRecord_00404c00 { unsigned char _pad[3]; unsigned char m_alpha; };
struct CThemeProvider_00404c00 {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026();
    virtual SThemeRecord_00404c00* GetTheme();     // slot 27 (+0x6c)
};
void __fastcall ImeUI_RenderCompositionString(bool a, bool b);   // 0x00CB38E0
struct CSprite { void DrawWithRenderManager(); };   // 0x0041A980
struct CFrontEndManager_00404c00 {
    unsigned char _pad[0xb8];
    CSprite*      m_pImeSprite;                    // +0xb8
    CSprite* GetImeSprite() { return m_pImeSprite; }
    static CFrontEndManager_00404c00* __fastcall GetInstance();   // 0x0041E5F2
};
struct CIME {
    unsigned char             _pad[8];
    bool                      m_active;            // +0x08
    unsigned char             _pad09[0x43];
    CThemeProvider_00404c00*  m_pTheme;            // +0x4c
    void Render();
};

void CIME::Render()
{
    if (m_active) {
        if (m_pTheme)
            g_imeCompositionAlpha_013750b0 = m_pTheme->GetTheme()->m_alpha;
        else
            g_imeCompositionAlpha_013750b0 = 0;
        bool flags = true;
        ImeUI_RenderCompositionString(flags, flags);
        CSprite* sprite = CFrontEndManager_00404c00::GetInstance()->GetImeSprite();
        if (sprite)
            sprite->DrawWithRenderManager();
    }
}