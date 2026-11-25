// NPlayerGui::CStatBar::DrawChangeText(const C2DVector&, const CWideString&, const CRGBColour&) -- draws a stat change text in the bar's font with the colour's alpha scaled by the game UI's global alpha
// Leaf: ?DrawChangeText@CStatBar@NPlayerGui@@IAEXABVC2DVector@@ABVCWideString@@ABVCRGBColour@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00645F2E, 95 bytes. Callees: __ftol2 @ 0x00BFEA70 (the scaled alpha),
// CGameUI::Draw2DText @ 0x00438010 (forwards to CDraw2DText::AddText @ 0x0064B6DA through the +0x238 member:
// pos, text, font, colour, scale, layer, flag). Global: the game UI singleton pointer @ 0x013B8790 (its float at
// +0x2A0 is the global UI alpha).
// Layout: CStatBar's font pointer at +0x60. CRGBColour is the PDB's B, G, R, A byte record; the faded colour is a
// local the compiler homes in the colour argument's dead slot ([ebp+10h], also the int slot for `fild`).
// Shape: frame pointer kept with pass-through `push dword ptr [ebp+n]` = size TU with frame-pointer omission off:
// #pragma optimize("y", off) + #pragma optimize("s", on). The colour constructor takes (r, g, b, a) and assigns R, G, B, A
// in its BODY (an init list stores in declaration order B, G, R, A; retail stores +2, +1, then A and B).
#pragma optimize("y", off)
#pragma optimize("s", on)
class C2DVector { public: float X, Y; };
class CWideString;
class CFontBank;

class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

enum EEngine2DLayer { ENGINE_2D_LAYER_TEXT = 0x43 };

struct CGameUI {
    unsigned char _pad00[0x2A0];
    float         m_globalAlpha;                  // +0x2A0
};
extern CGameUI* g_pGameUI;                         // 0x013B8790

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x00438010 ?Draw2DText@CPlayerGui@@QAEXABVC2DVector@@ABVCWideString@@ABVCFontBank@@ABVCRGBColour@@MW4EEngine2DLayer@@_N@Z (was ?Draw2DText@CGameUI@@QAEXABVC2DVector@@ABVCWideString@@PAVCFontBank@@ABVCRGBColour@@MW4EEngine2DLayer@@_N@Z)
class CPlayerGui { public: void Draw2DText(C2DVector const&, CWideString const&, CFontBank const&, CRGBColour const&, float, EEngine2DLayer, bool); };
namespace NPlayerGui {

class CStatBar { public:
    unsigned char _pad00[0x60];
    CFontBank*    m_pFont;                         // +0x60
    protected: void DrawChangeText(const C2DVector& pos, const CWideString& text, const CRGBColour& colour);
};

void CStatBar::DrawChangeText(const C2DVector& pos, const CWideString& text, const CRGBColour& colour)
{
    CRGBColour faded(colour.R, colour.G, colour.B, (unsigned char)(colour.A * g_pGameUI->m_globalAlpha));
    reinterpret_cast<CPlayerGui*>(g_pGameUI)->Draw2DText(pos, text, *(m_pFont), faded, 1.0f, ENGINE_2D_LAYER_TEXT, true);
}

}