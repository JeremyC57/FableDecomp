// Behaviour test for NPlayerGui::CDrawTargetInfo::OnEventAttack @ 0x00644526
#include <cstdio>
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) : B(other.B), G(other.G), R(other.R), A(other.A) {}
};
struct CPlayerGuiDef { float ABXYButtonFadeDurationSeconds; };
static CPlayerGuiDef g_def; static int g_peeks = 0;
static CPlayerGuiDef* PeekGuiDef() { ++g_peeks; return &g_def; }
struct CFadeInFadeOutBase {
    bool Active; float FadeInTime, FadeOutTime; bool Closing, Opening; float OpenTimer, CloseTimer; unsigned char ToR, ToG, ToB, ToA;
    void Start(float fadeInTime, float fadeOutTime, CRGBColour toColour)
    { Active = true; Opening = true; Closing = false; FadeInTime = fadeInTime; FadeOutTime = fadeOutTime; ToR = toColour.R; ToG = toColour.G; ToB = toColour.B; ToA = toColour.A; }
};
struct CDrawTargetInfo {
    CFadeInFadeOutBase AttackAlphaFade, BlockAlphaFade;
    void OnEventAttack()
    {
        AttackAlphaFade.Start(PeekGuiDef()->ABXYButtonFadeDurationSeconds, 0.0f, CRGBColour(0, 0, 0, 255));
        BlockAlphaFade.Start(PeekGuiDef()->ABXYButtonFadeDurationSeconds, 0.0f, CRGBColour(0, 0, 0, 255));
    }
};
int main()
{
    g_def.ABXYButtonFadeDurationSeconds = 0.75f;
    CDrawTargetInfo d;
    d.AttackAlphaFade.Active = false; d.BlockAlphaFade.Active = false;
    d.OnEventAttack();
    if (g_peeks != 2 || !d.AttackAlphaFade.Active || !d.BlockAlphaFade.Opening || d.AttackAlphaFade.FadeInTime != 0.75f ||
        d.BlockAlphaFade.FadeOutTime != 0.0f || d.AttackAlphaFade.ToA != 255 || d.BlockAlphaFade.ToR != 0) {
        std::printf("00644526_TEST FAIL\n"); return 1;
    }
    std::printf("00644526_TEST PASS\n");
    return 0;
}
