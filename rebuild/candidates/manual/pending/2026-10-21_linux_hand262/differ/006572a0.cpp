// Grade: DIFFER(462v462) -- frame slot only: the font's by-value 4-byte text-extent return lands at [esp+0x1c] (shared with the `width` local) where retail gives it its own word at [esp+0x24]; every other byte matches.
// NPlayerGui::CDrawNPCName::Draw(const CEnginePrimitivePrepareInfo&) -- draw the NPC name centred at the top of the screen (static white colour,
// scale 0.7, text layer); for 25 ticks after a screenshot also draw TEXT_GUI_MENU_SCREENSHOT_TAKEN (fallback L"Screenshot taken") at (280, 35).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006572A0, length 464 bytes (0x1cc code), `ret 4`.
// Callees: CWideString GetLength 0x0099B220 / GetString 0x0099AF10 / operator=(const wchar_t*) 0x0099B800 / dtor 0x0099B510; font vslot 1
// (text extent, by value); CDisplayManager::GetRenderTargetDimensions 0x009BEDC0 on g_pDisplayDevice 0x013B8390; GFAXToVX 0x009E1DA0 /
// GFAYToVY 0x009E1DD0; CGameUI::Draw2DText 0x00438010 (this +4, landed 0x00645F2E naming); CMainGameComponent tick getter 0x004167C8 on
// 0x013B86A0; CDataBank::GetTextBySymbol 0x009C95E0 (+0x14); CCharString 0x0099EBF0/0x0099EAE0. Static colour 0x013BA864 (guard 0x013BA868).
// Constants 0.5f 0x0122F59C, 25.0f 0x0123003C. Layout: game UI +4, owner +0x10 (font +0x1614C), drawn +0x18, name +0x28, screenshot tick +0x2c.
// Tried: single-expression x, 8-byte extent (slot +0x20), const-ref extent, copy-init / assigned CTextExtent local with a user copy ctor.
// Leaf: Draw
#include <math.h>
#pragma intrinsic(fabs)
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C2DExtentsI { long X, Y; };
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CCharString {
    char* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CWideString {
    void* m_pData;
    long GetLength() const;                        // 0x0099B220
    const unsigned short* GetString(long start = 0) const;   // 0x0099AF10
    CWideString& operator=(const unsigned short* text);   // 0x0099B800
    ~CWideString();                                // 0x0099B510
};
struct CTextExtent {
    float Width;
    CTextExtent();
};
struct CFontBank {
    virtual void v00();
    virtual CTextExtent GetTextExtent(const unsigned short* text);   // slot 1 (+0x04)
};
enum EEngine2DLayer { ENGINE_2D_LAYER_TEXT = 0x43 };
struct CGameUI {
    void Draw2DText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                    float scale, EEngine2DLayer layer, bool flag);   // 0x00438010
};
struct CDisplayManager {
    C2DExtentsI GetRenderTargetDimensions() const;  // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayDevice;           // 0x013B8390
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CDataBank* PDataBank;                           // +0x14
    long GetGameTick() const;                       // 0x004167C8
};
extern CMainGameComponent* g_pMainGameComponent;    // 0x013B86A0
float __fastcall GFAYToVY(float y);                 // 0x009E1DD0 (value in a stack slot)
float __fastcall GFAXToVX(float x);                 // 0x009E1DA0
struct CPlayerGuiOwner {
    unsigned char _pad00[0x1614c];
    CFontBank* PFont;                               // +0x1614c
};
struct CEnginePrimitivePrepareInfo;

namespace NPlayerGui {
struct CDrawNPCName {
    virtual void v00();
    CGameUI* PGameUI;                               // +0x04
    unsigned char _pad08[8];
    CPlayerGuiOwner* POwner;                        // +0x10
    unsigned char _pad14[4];
    bool Drawn;                                     // +0x18
    unsigned char _pad19[0x28 - 0x19];
    CWideString Name;                               // +0x28
    long ScreenshotTime;                            // +0x2c
    void Draw(const CEnginePrimitivePrepareInfo& info);
};

void CDrawNPCName::Draw(const CEnginePrimitivePrepareInfo& info)
{
    if (Name.GetLength() > 0) {
        Drawn = true;
        CFontBank* font = POwner->PFont;
        static CRGBColour colour(0xff, 0xff, 0xff, 0xff);
        float width = font->GetTextExtent(Name.GetString()).Width;
        float x = g_pDisplayDevice->GetRenderTargetDimensions().X * 0.5f - width * 0.5f;
        PGameUI->Draw2DText(C2DVector(GFAXToVX(x), GFAYToVY(10.0f)), Name, font, colour, 0.7f, ENGINE_2D_LAYER_TEXT, true);
    }
    if (ScreenshotTime > 0 && fabs((float)(g_pMainGameComponent->GetGameTick() - ScreenshotTime)) < 25.0f) {
        Drawn = true;
        CFontBank* font = POwner->PFont;
        CRGBColour colour(0xff, 0xff, 0xff, 0xff);
        CWideString text = g_pMainGameComponent->PDataBank->GetTextBySymbol("TEXT_GUI_MENU_SCREENSHOT_TAKEN");
        if (text.GetLength() == 0)
            text = L"Screenshot taken";
        PGameUI->Draw2DText(C2DVector(GFAXToVX(280.0f), GFAYToVY(35.0f)), text, font, colour, 0.7f, ENGINE_2D_LAYER_TEXT, true);
    }
}
}
