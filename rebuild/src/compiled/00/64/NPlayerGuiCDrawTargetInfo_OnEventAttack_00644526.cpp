#pragma optimize("s", on)
// NPlayerGui::CDrawTargetInfo::OnEventAttack() -- on an attack event restart both the attack and the block alpha fades: fade in over the GUI def's ABXY button fade duration, no fade-out time, towards opaque black
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00644526, length 97 bytes.
// Callees: PeekGuiDef @ 0x00438570 (`__fastcall`, no arguments, CPlayerGuiDef*), CFadeInFadeOutBase::Start(float fadeInTime, float fadeOutTime,
// CRGBColour toColour) @ 0x00434840 (thiscall on each fade; it sets Active/Opening, clears Closing and stores the times and the colour; the
// manifest label "CEngine::StartNewScene" is propagated, the method name is descriptive). Float pool: none (`fldz` = 0.0f).
// Layout: CDrawTargetInfo AttackAlphaFade +0x28, BlockAlphaFade +0x44 (landed constructor 0x0064DAB4); the def's float at retail +0x7cc is
// taken to be ABXYButtonFadeDurationSeconds (PDB +0x814, the only float in that part of CPlayerGuiDef; the retail def is smaller).
// Shape: a size TU (unaligned entry, `push ecx` argument slots); arguments evaluated right to left: the by-value CRGBColour built in its
// slot (it has a user copy constructor) by its user constructor (R +2, G +1, B +0, A +3: a B,G,R,A layout assigned r, g, b, a in the
// constructor body; an initialiser list stores in declaration order; the zeros from bl), then the
// `fldz` 0.0f, then PeekGuiDef() and the duration through the x87; the def is fetched again for the second fade.
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
    CRGBColour(const CRGBColour& other) : B(other.B), G(other.G), R(other.R), A(other.A) {}
};

struct CPlayerGuiDef {
    unsigned char _pad[0x7cc];
    float         ABXYButtonFadeDurationSeconds;   // +0x7cc (retail)
};
extern CPlayerGuiDef* __fastcall PeekGuiDef();     // 0x00438570

struct CFadeInFadeOutBase {
    bool       Active;                       // +0x00
    float      FadeInTime;                   // +0x04
    float      FadeOutTime;                  // +0x08
    bool       Closing;                      // +0x0c
    bool       Opening;                      // +0x0d
    float      OpenTimer;                    // +0x10
    float      CloseTimer;                   // +0x14
    CRGBColour ToColour;                     // +0x18
    void Start(float fadeInTime, float fadeOutTime, CRGBColour toColour);   // 0x00434840
};

namespace NPlayerGui {

struct CDrawTargetInfo {
    void*              __vftable;            // +0x00
    unsigned char      _base[0x28 - 4];      // CDrawBase
    CFadeInFadeOutBase AttackAlphaFade;      // +0x28
    CFadeInFadeOutBase BlockAlphaFade;       // +0x44
    void OnEventAttack();
};

void CDrawTargetInfo::OnEventAttack()
{
    AttackAlphaFade.Start(PeekGuiDef()->ABXYButtonFadeDurationSeconds, 0.0f, CRGBColour(0, 0, 0, 255));
    BlockAlphaFade.Start(PeekGuiDef()->ABXYButtonFadeDurationSeconds, 0.0f, CRGBColour(0, 0, 0, 255));
}

}