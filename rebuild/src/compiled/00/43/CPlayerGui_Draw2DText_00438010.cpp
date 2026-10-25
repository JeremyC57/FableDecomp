// CPlayerGui::Draw2DText(const C2DVector& pos, const CWideString& text, const CFontBank& font, const CRGBColour& colour, float scale, EEngine2DLayer layer, bool flag)
// -- forward a 2D text request to the GUI's text drawer: PDraw2DText->AddText(...) with the same arguments
// Leaf: ?Draw2DText@CPlayerGui@@QAEXABVC2DVector@@ABVCWideString@@ABVCFontBank@@ABVCRGBColour@@MW4EEngine2DLayer@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00438010, 11 bytes: `mov ecx,[ecx+238h] ; jmp 0x0064B6DA` (the seven stack
//   arguments are passed through in place).
// Renamed from CGameUI::Draw2DText (FINDINGS chunk 382: the landed model was a generic forwarder; no CGameUI class exists in
//   Ego_r.pdb or FableWin.pdb). Name: Ego_r.pdb ?Draw2DText@CPlayerGui@@QAEXABVC2DVector@@ABVCWideString@@ABVCFontBank@@
//   ABVCRGBColour@@MW4EEngine2DLayer@@_N@Z, the same parameter list as its target, the landed
//   NPlayerGui::CDraw2DText::AddText 0x0064B6DA (?AddText@CDraw2DText@NPlayerGui@@QAEXABVC2DVector@@...). Callers are the
//   NPlayerGui::CDraw* elements (CDrawSpiritScores, CDrawHealth, CDrawMiniMap, CStatBar, ...) and CSubtitleRenderer.
// Layout: struct_layouts_egor CPlayerGui::PDraw2DText (CCountedPointer<NPlayerGui::CDraw2DText>, Data first) at PDB +0x240;
//   retail +0x238 because the embedded CSubtitleRenderer (+0x18) is 8 bytes smaller in retail (six 12-byte vectors, a
//   0x28-byte deque). The engine header uses the PDB offsets, so a local model is used.
class C2DVector;
class CWideString;
class CFontBank;
class CRGBColour;
class CCPPointerInfo;
enum EEngine2DLayer { ENGINE_2D_LAYER_0 };

namespace NPlayerGui {
class CDraw2DText {
public:
    void AddText(const C2DVector& pos, const CWideString& text, const CFontBank& font, const CRGBColour& colour,
                 float scale, EEngine2DLayer layer, bool flag);                   // 0x0064B6DA
};
}

template <class T> class CCountedPointer {
public:
    T*              Data;                                                      // +0x00
    CCPPointerInfo* Info;                                                      // +0x04
    T* operator->() const { return Data; }
};

class CPlayerGui {
public:
    unsigned char _pad_0x000[0x238];
    CCountedPointer<NPlayerGui::CDraw2DText> PDraw2DText;                       // +0x238 (PDB +0x240)
    void Draw2DText(const C2DVector& pos, const CWideString& text, const CFontBank& font, const CRGBColour& colour,
                    float scale, EEngine2DLayer layer, bool flag);
};

void CPlayerGui::Draw2DText(const C2DVector& pos, const CWideString& text, const CFontBank& font, const CRGBColour& colour,
                            float scale, EEngine2DLayer layer, bool flag)
{
    PDraw2DText->AddText(pos, text, font, colour, scale, layer, flag);
}
