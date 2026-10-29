// NPlayerGui::CStatBar::DrawStatBoxWithText(const CEnginePrimitivePrepareInfo& info, std::vector<CEnginePrimitiveHandle>& handles, const C2DVector& pos, long graphic, const CWideString& text, long value, unsigned char alpha, bool rightAlign) -- draw a stat box, then its text at the money-text offset (right-aligned to the screen edge when asked)
// Leaf: ?DrawStatBoxWithText@CStatBar@NPlayerGui@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00646A46, 204 bytes, `ret 18h` (static __fastcall: info in ecx, handles in
// edx, six stack arguments). Size-optimised NPlayerGui TU (frame pointer, `leave`).
// Identity: Ego_r.pdb `?DrawStatBoxWithText@CStatBar@NPlayerGui@@SIXABVCEnginePrimitivePrepareInfo@@AAV?$vector@
// VCEnginePrimitiveHandle@@...@std@@ABVC2DVector@@JABVCWideString@@JE_N@Z`.
// Callees: the static CStatBar::DrawStatBox 0x00645853 (__fastcall, info/handles passed through in ecx/edx; the landed
// 0x00645CE9 declaration), PeekGuiDef 0x00438570, GFACoordToVCoord(const C2DVector&, C2DVector&) 0x009E21D0 (__fastcall),
// CDisplayManager::GetRenderTargetDimensions 0x009BEDC0 (display 0x013B8390), GFAXToVX(float) 0x009E1DA0,
// CFontBank::GetVStringWidth 0x0043E4D0, CFontBank::Preload (virtual slot 4, +0x10; result dropped), CGameUI::Draw2DText
// 0x00438010 (game UI 0x013B8790; the landed DrawChangeText 0x00645F2E declaration).
// Layout: CGameUI Font +0x138 (the landed DrawStatBox model); CPlayerGuiDef MoneyTextOffset at +0x4AC (Ego_r.pdb +0x4DC,
// twelve PDB vectors 4 bytes wider before it); CRGBColour is the B, G, R, A byte record (constructor r, g, b, a).
// Shape: the font is read before PeekGuiDef; the actual-screen text position is a NAMED local built with the (x, y)
// constructor as `offset + pos` per coordinate (Y first, right to left; a temporary would share the later dimensions
// slot and the inline operator+ computes X first), converted into a second named local; the right-align arithmetic parks
// a partial in the dead `rightAlign` slot, the white colour temporary reuses that slot.
#pragma optimize("s", on)
class C2DVector { public:
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};

class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

class CWideString;
class CEnginePrimitivePrepareInfo;
class CEnginePrimitiveHandle { public: unsigned long m_handle; };

namespace std {
template <class T> class allocator {};
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}

class C2DExtentsI { public: long Width, Height; };
class CDisplayManager { public: const C2DExtentsI GetRenderTargetDimensions(); };   // 0x009BEDC0
extern CDisplayManager* g_pDisplayManager;                              // 0x013B8390

void __fastcall GFACoordToVCoord(const C2DVector& in, C2DVector& out);  // 0x009E21D0
float __fastcall GFAXToVX(float x);                                     // 0x009E1DA0

class CFontBank {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual bool Preload(const CWideString& text, bool wait) const;     // slot 4 (+0x10)
    float GetVStringWidth(const CWideString& text) const;               // 0x0043E4D0
};

enum EEngine2DLayer { ENGINE_2D_LAYER_TEXT = 0x43 };

struct CGameUI {
    unsigned char _pad[0x138];
    CFontBank*    Font;                                                 // +0x138
    void Draw2DText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                    float scale, EEngine2DLayer layer, bool flag);      // 0x00438010
};
extern CGameUI* g_pGameUI;                                              // 0x013B8790

class CPlayerGuiDef { public:
    unsigned char _pad[0x4ac];
    C2DVector     MoneyTextOffset;                                      // +0x4AC (retail)
};
extern const CPlayerGuiDef* __fastcall PeekGuiDef();                    // 0x00438570

namespace NPlayerGui {

class CStatBar {
public:
    static void __fastcall DrawStatBox(const CEnginePrimitivePrepareInfo& info, std::vector<CEnginePrimitiveHandle>& handles,
        const C2DVector& pos, long graphic, const CWideString& text, long value, unsigned char alpha, bool withText, bool flag,
        bool extra);                                                    // 0x00645853
    static void __fastcall DrawStatBoxWithText(const CEnginePrimitivePrepareInfo& info,
        std::vector<CEnginePrimitiveHandle>& handles, const C2DVector& pos, long graphic, const CWideString& text, long value,
        unsigned char alpha, bool rightAlign);
};

void __fastcall CStatBar::DrawStatBoxWithText(const CEnginePrimitivePrepareInfo& info,
    std::vector<CEnginePrimitiveHandle>& handles, const C2DVector& pos, long graphic, const CWideString& text, long value,
    unsigned char alpha, bool rightAlign)
{
    DrawStatBox(info, handles, pos, graphic, text, value, alpha, rightAlign, false, true);
    CFontBank* font = g_pGameUI->Font;
    const CPlayerGuiDef* def = PeekGuiDef();
    C2DVector screenPos(def->MoneyTextOffset.X + pos.X, def->MoneyTextOffset.Y + pos.Y);
    C2DVector textPos;
    GFACoordToVCoord(screenPos, textPos);
    if (rightAlign)
        textPos.X = GFAXToVX((float)g_pDisplayManager->GetRenderTargetDimensions().Width) - textPos.X
                  - font->GetVStringWidth(text);
    font->Preload(text, false);
    g_pGameUI->Draw2DText(textPos, text, font, CRGBColour(255, 255, 255, 255), 1.0f, ENGINE_2D_LAYER_TEXT, false);
}

}