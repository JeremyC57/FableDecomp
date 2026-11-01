// NPlayerGui::CDrawNPCName::Draw(const CEnginePrimitivePrepareInfo& prepare_info) -- draw the NPC name centred at the top of the screen (static white colour, scale 0.7, text layer); while the screenshot message shows, also draw TEXT_GUI_MENU_SCREENSHOT_TAKEN (fallback L"Screenshot taken") at (280, 35)
// Leaf: ?Draw@CDrawNPCName@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Naming wave 18: PDisplayManager is GPDisplayManager (?GPDisplayManager@@3PAVCDisplayManager@@A); ChatGPT pass 10 audit, raw Ego_r public.
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006572A0, 462 bytes (+2 int3), `ret 4`. Ego_r.pdb
//   ?Draw@CDrawNPCName@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z.
// Callees: CWideString::GetLength 0x0099B220, GetAsCharArray 0x0099AF10, operator=(const wchar_t*) 0x0099B800, ~CWideString
//   0x0099B510; CFontBank virtual slot 1 GetStringExtents(const wchar_t*, bool) const (C2DExtentsF by hidden pointer, the
//   slot the landed CFontBank::GetVStringExtents 0x00437210 calls); CDisplayManager::GetRenderTargetDimensions 0x009BEDC0 on
//   0x013B8390; GFAXToVX 0x009E1DA0 / GFAYToVY 0x009E1DD0; CPlayerGui::Draw2DText 0x00438010 (landed);
//   CMainGameComponent::GetCurrentWorldFrame 0x004167C8 (a tail `jmp` to CWorld::GetFrame 0x0049D870);
//   NGameText::CDataBank::GetTextBySymbol 0x009C95E0; CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0. Static colour
//   0x013BA864 (guard 0x013BA868). Constants 0.5f 0x0122F59C, 25.0f 0x0123003C.
// Layout (retail, local model): CDrawBase {vptr, PlayerGui +4, PlayerManager +8, World +0xc, Component +0x10, Bank +0x14,
//   Drawing +0x18, PrimitiveHandles (STLport vector, 12 bytes) +0x1c}; CDrawNPCName Name +0x28, ScreenShotMessageCounter +0x2c
//   (Ego_r.pdb has +0x2c/+0x30: its Dinkumware vector is 16 bytes). CMainGameComponent PTextBank +0x14,
//   PCutSceneMainFont +0x1614c (shared engine/CCountedPointer.h). No engine header exists for CDrawNPCName / CDrawBase;
//   engine/CFontBank.h and engine/CPlayerGui.h flatten the vptr / use PDB offsets, so local views are used for them.
// Shape: the width comes from the PDB inline CFontBank::GetStringWidth(const CWideString&) = GetStringExtents(text.
//   GetAsCharArray(), false).Width. The 8-byte C2DExtentsF return (non-POD, hidden pointer) is what gives retail its frame:
//   the extents temporary shares the render-target dimensions' slot [esp+0x20] and the width/x float takes [esp+0x1c]. The
//   earlier 4-byte "text extent" model (with GetString taking the bool push as an argument) shared the width slot.
#include <math.h>
#pragma intrinsic(fabs)

class C2DVector {
public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class C2DExtentsI {
public:
    long Width, Height;
};
class C2DExtentsF {
public:
    float Width;
    float Height;
    C2DExtentsF() {}
    C2DExtentsF(float width, float height) : Width(width), Height(height) {}
};
class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                             // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
    long GetLength() const;                                     // 0x0099B220
    const unsigned short* GetAsCharArray() const;               // 0x0099AF10
    const CWideString& operator=(const unsigned short* text);   // 0x0099B800 (Ego_r ??4CWideString@@QAEABV0@PB_W@Z)
    ~CWideString();                                             // 0x0099B510
};
class CFontManager;
class CFontBank {
public:
    unsigned long FontHeight;                                   // +0x04
    void*         Name_PStringData;                             // +0x08
    CFontManager* PManager;                                     // +0x0c
    virtual ~CFontBank();
    virtual C2DExtentsF GetStringExtents(const unsigned short* text, bool wrap) const;   // slot 1 (+0x04)
    float GetStringWidth(const CWideString& text) const { return GetStringExtents(text.GetAsCharArray(), false).Width; }
};
enum EEngine2DLayer { ENGINE_2D_LAYER_TEXT = 0x43 };
class CPlayerGui {
public:
    void Draw2DText(const C2DVector& pos, const CWideString& text, const CFontBank& font, const CRGBColour& colour,
                    float scale, EEngine2DLayer layer, bool flag);   // 0x00438010
};
class CDisplayManager {
public:
    const C2DExtentsI GetRenderTargetDimensions();              // 0x009BEDC0
};
extern CDisplayManager* GPDisplayManager;                     // 0x013B8390
namespace NGameText {
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
#include "engine/CCountedPointer.h"
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private:
public:
    unsigned char _pad00[0x14];
    CCountedPointer<NGameText::CDataBank> PTextBank;            // +0x14
    unsigned char _pad1c[0x1614c - 0x1c];
    CCountedPointer<CFontBank> PCutSceneMainFont;               // +0x1614c
    long GetCurrentWorldFrame() const;                          // 0x004167C8
    const NGameText::CDataBank& GetTextBank() const { return *PTextBank; }
    const CFontBank& PeekCutSceneMainFont() const { return *PCutSceneMainFont; }
};
float __fastcall GFAXToVX(float x);                             // 0x009E1DA0
float __fastcall GFAYToVY(float y);                             // 0x009E1DD0
class CPlayerManager;
class CWorld;
class CGraphicDataBank;
class CEnginePrimitivePrepareInfo;

namespace NPlayerGui {

class CDrawNPCName {
public:
    virtual ~CDrawNPCName();
    CPlayerGui*         PlayerGui;                              // +0x04
    CPlayerManager*     PlayerManager;                          // +0x08
    CWorld*             World;                                  // +0x0c
    CMainGameComponent* Component;                              // +0x10
    CGraphicDataBank*   Bank;                                   // +0x14
    bool                Drawing;                                // +0x18
    unsigned char       PrimitiveHandles[0x0c];                 // +0x1c std::vector<CEnginePrimitiveHandle> (STLport)
    CWideString         Name;                                   // +0x28
    long                ScreenShotMessageCounter;               // +0x2c
    void Draw(const CEnginePrimitivePrepareInfo& prepare_info);
};

void CDrawNPCName::Draw(const CEnginePrimitivePrepareInfo& prepare_info)
{
    if (Name.GetLength() > 0) {
        Drawing = true;
        const CFontBank& font = Component->PeekCutSceneMainFont();
        static CRGBColour colour(0xff, 0xff, 0xff, 0xff);
        float width = font.GetStringWidth(Name);
        float x = GPDisplayManager->GetRenderTargetDimensions().Width * 0.5f - width * 0.5f;
        PlayerGui->Draw2DText(C2DVector(GFAXToVX(x), GFAYToVY(10.0f)), Name, font, colour, 0.7f, ENGINE_2D_LAYER_TEXT, true);
    }
    if (ScreenShotMessageCounter > 0 &&
        fabs((float)(CMainGameComponent::Get()->GetCurrentWorldFrame() - ScreenShotMessageCounter)) < 25.0f) {
        Drawing = true;
        const CFontBank& font = Component->PeekCutSceneMainFont();
        CRGBColour colour(0xff, 0xff, 0xff, 0xff);
        CWideString text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_SCREENSHOT_TAKEN");
        if (text.GetLength() == 0)
            text = L"Screenshot taken";
        PlayerGui->Draw2DText(C2DVector(GFAXToVX(280.0f), GFAYToVY(35.0f)), text, font, colour, 0.7f, ENGINE_2D_LAYER_TEXT, true);
    }
}

}