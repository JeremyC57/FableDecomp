// NPlayerGui::CCrimeStatBar::DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos, const C2DVector& textOffset, const CRGBColour& colour, unsigned char alpha, bool withText) -- the crime bar grows upward: round the bar end (lerped from the def's left offset by the fill fraction) to a whole pixel, flip it against the render-target height, draw the stat box with the value text, then centre the value text horizontally over the bar in the def's text colour when the bar is flagged
// Grade: RELOCATION_MATCH (retail_asm grade: the FloorToLong inline-asm helper as the landed ModifyHealth 0x006A68F0)
// Leaf: DrawStatBarOnLeft@CCrimeStatBar
// Naming wave 18: PDisplayManager is GPDisplayManager (?GPDisplayManager@@3PAVCDisplayManager@@A); ChatGPT pass 10 audit, raw Ego_r public.
// Retail-asm: 0x00646044..0x0064605E `fld [ebp-0Ch] ; fsub [ebp-8] ; fistp dword ptr [ebp+10h] ; fild [ebp+10h] ; fadd [ebp-4] ; fstp [ebp-10h] ; mov eax,[ebp-0Ch] ; cmp eax,[ebp-10h] ; jne ; inc dword ptr [ebp+10h]` -- the engine's inline-assembly floor helper expanded in place (0.5f / 1.0f frame constants, a bare dword fistp; same block as GFFloatToLongFloor 0x0048D940, the landed CThingPhysical::ModifyHealth 0x006A68F0 and the sibling CStatBar::DrawStatBarOnLeft 0x00646762); VC7.1 emits no bare dword fistp from C++.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongFloor.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00646010, 399 bytes, `ret 1Ch`, frame-pointer TU (`leave`).
// Signature: Ego_r.pdb ?DrawStatBarOnLeft@CCrimeStatBar@NPlayerGui@@UAEXABVCEnginePrimitivePrepareInfo@@ABVCGraphicDataBank@@ABVC2DVector@@2ABVCRGBColour@@E_N@Z.
// Callees: CDisplayManager::GetRenderTargetDimensions 0x009BEDC0, CWideString from long 0x00405F00 (__fastcall), __ftol2,
//   DrawStatBox (virtual slot 2), ~CWideString 0x0099B510, CFontBank::GetStringWidth 0x0048DA62, GFACoordToVCoord 0x009E21D0,
//   CPlayerGui::Draw2DText 0x00438010 (Ego_r).  Float pool: 0.5f @ 0x0122F59C.
// Layout: as the sibling CStatBar::DrawStatBarOnLeft 0x00646762 TU (retail = Ego_r.pdb CStatBar minus 8); CCrimeStatBar adds the
//   text-colour flag at +0x68 and its def the text colour at +0x424.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
// the engine's inline-assembly FPU floor helper (standalone copy GFFloatToLongFloor 0x0048D940; same block as the landed
// CThingPhysical::ModifyHealth 0x006A68F0 FloorToLong): VC7.1 expands the __forceinline body in place; the by-value
// parameter and the four locals become frame words, which is why the function carries an ebp frame.
#include "engine/GFFloatToLongFloor.h"
class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class CRGBColour { public:                                                     // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour() {}
    FABLE_FORCEINLINE CRGBColour(const CRGBColour& other) { G = other.G; R = other.R; B = other.B; A = other.A; }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CWideString {
public:
    ~CWideString();                                                     // 0x0099B510
    void* Data;
};
CWideString __fastcall LongToWideString(long value);                    // 0x00405F00
class CEnginePrimitivePrepareInfo { public: unsigned long _pad0; float FrameFraction; };
class CGraphicDataBank;
void __fastcall GFACoordToVCoord(const C2DVector& in, C2DVector& out);  // 0x009E21D0
class C2DExtentsI { public: long Width, Height; };
class CDisplayManager { public: const C2DExtentsI GetRenderTargetDimensions(); };   // 0x009BEDC0
extern CDisplayManager* GPDisplayManager;                    // 0x013B8390
class CFontBank { public: float GetStringWidth(const CWideString& text) const; };   // 0x0048DA62
enum EEngine2DLayer { ENGINE_2D_LAYER_SUBTITLE = 0x43 };
struct CPlayerGui {
    void Draw2DText(const C2DVector& pos, const CWideString& text, const CFontBank& font, const CRGBColour& colour, float scale, EEngine2DLayer layer, bool flag);   // 0x00438010
};
extern CPlayerGui* g_pGameUI;                                              // 0x013B8790
struct CCrimeStatBarDef {
    unsigned char   _pad000[0x424];
    CRGBColour      TextColour;                                         // +0x424
    unsigned char   _pad428[0x4bc - 0x428];
    float           BarLeftOffset;                                      // +0x4bc
};
namespace NPlayerGui {
class CCrimeStatBar { public:
    virtual void v0();
    virtual void v1();
    virtual void DrawStatBox(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos, long type, long effect, long graphic, const CWideString& text, unsigned char alpha, bool withText, bool flag);
    CCrimeStatBarDef* Def;                                              // +0x04
    unsigned char _pad08[0x10 - 0x08];
    float         Value;                                                // +0x10
    unsigned char _pad14[0x2c - 0x14];
    float         Fraction;                                             // +0x2c
    unsigned char _pad30[0x50 - 0x30];
    long          StatType;                                             // +0x50
    long          Effect;                                               // +0x54
    long          Graphic;                                              // +0x58
    unsigned char _pad5c[0x60 - 0x5c];
    CFontBank*    Font;                                                 // +0x60
    unsigned char _pad64[0x68 - 0x64];
    long          TextColourFlag;                                       // +0x68
    void DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos, const C2DVector& textOffset, const CRGBColour& colour, unsigned char alpha, bool withText);
};

void CCrimeStatBar::DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos, const C2DVector& textOffset, const CRGBColour& colour, unsigned char alpha, bool withText)
{
    float left = -Def->BarLeftOffset;
    float y = (pos.Y - left) * Fraction + left;
    long rounded = GFFloatToLongFloor(y);
    C2DVector barEnd(pos.X, (float)rounded);
    barEnd.Y = (float)GPDisplayManager->GetRenderTargetDimensions().Height - barEnd.Y;
    DrawStatBox(info, bank, barEnd, StatType, Effect, Graphic, LongToWideString((long)Value), alpha, withText, true);
    CRGBColour textColour = colour;
    if (TextColourFlag > 0)
        textColour = Def->TextColour;
    C2DVector textPos(0.0f, barEnd.Y + textOffset.Y);
    float width = (float)GPDisplayManager->GetRenderTargetDimensions().Width;
    textPos.X = (width - Font->GetStringWidth(LongToWideString((long)Value))) * 0.5f;
    C2DVector screen;
    GFACoordToVCoord(textPos, screen);
    CPlayerGui* gameUI = g_pGameUI;
    gameUI->Draw2DText(screen, LongToWideString((long)Value), *Font, CRGBColour(textColour.R, textColour.G, textColour.B, alpha), 1.0f, ENGINE_2D_LAYER_SUBTITLE, false);
}
}