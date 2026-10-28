// NPlayerGui::CDrawTargetInfo::OnEventAttack() -- on an attack event restart the attack alpha fade: fade in over the GUI def's ABXY button fade duration, no fade-out time, towards opaque black
// Leaf: ?OnEventAttack@CDrawTargetInfo@NPlayerGui@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00644587, 51 bytes, thiscall, `ret`, /O1 TU.
// Identity (proven): Ego_r.pdb donor order and sizes OnEventUnsheatheWeapon 0x006875D6 (0x61 = 97 bytes) -> OnEventAttack
//   0x00687637 (0x33 = 51 bytes); retail 0x00644526 is 97 bytes (both fades) and this row 51 bytes (one fade). The landed
//   0x00644526 carries ?OnEventAttack@CDrawTargetInfo@NPlayerGui@@QAEXXZ and is really OnEventUnsheatheWeapon -> Review at landing.
// Callees: CPlayerGui::PeekGuiDef() 0x00438570 (static __fastcall, const CPlayerGuiDef&), CFadeInFadeOutBase::Open(float, float,
//   CRGBColour) 0x00434840 (landed spellings). Layout and colour model from the landed twin 0x00644526.
#pragma optimize("s", on)
class CRGBColour { public:
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

class CPlayerGuiDef { public:
    unsigned char _pad[0x7cc];
    float         ABXYButtonFadeDurationSeconds;   // +0x7cc (retail)
};
class CPlayerGui {
public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();   // 0x00438570
};     // 0x00438570

class CFadeInFadeOutBase { public:
    bool       Active;                       // +0x00
    float      FadeInTime;                   // +0x04
    float      FadeOutTime;                  // +0x08
    bool       Closing;                      // +0x0c
    bool       Opening;                      // +0x0d
    float      OpenTimer;                    // +0x10
    float      CloseTimer;                   // +0x14
    CRGBColour ToColour;                     // +0x18
public:
    void Open(float, float, CRGBColour);   // 0x00434840
};

namespace NPlayerGui {

class CDrawTargetInfo { public:
    void*              __vftable;            // +0x00
    unsigned char      _base[0x28 - 4];      // CDrawBase
    CFadeInFadeOutBase AttackAlphaFade;      // +0x28
    CFadeInFadeOutBase BlockAlphaFade;       // +0x44
    void OnEventAttack();
};

void CDrawTargetInfo::OnEventAttack()
{
    AttackAlphaFade.Open(CPlayerGui::PeekGuiDef().ABXYButtonFadeDurationSeconds, 0.0f, CRGBColour(0, 0, 0, 255));
}

}