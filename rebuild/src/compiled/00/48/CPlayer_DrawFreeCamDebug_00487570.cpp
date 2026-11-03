// CPlayer::DrawFreeCamDebug(const C2DBoxF* box, const CFontBank& font) const -- debug overlay of the free camera: in the given virtual window, draw "Freecam Info", then rows "CameraPos" x y z, "CameraVec" x y z (each rounded to two places) and "FOV" (whole degrees), the values spaced by the width of the title
// Leaf: ?DrawFreeCamDebug@CPlayer@@QBEXPBVC2DBoxF@@ABVCFontBank@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00487570, 1305 bytes, thiscall `ret 8`, 8-aligned frame (the double argument of
//   GFFloatToWideString). Identity: Ego_r.pdb / FableWin.pdb ?DrawFreeCamDebug@CPlayer@@QBEXPBVC2DBoxF@@ABVCFontBank@@@Z.
// Callees: CCamera::GetHorizontalFOV 0x00A0BE90 (on CurrentFreeCamera +0x40), __ftol2, CFontBank virtuals GetFontMaxHeight (slot 2)
//   and GetStringExtents (slot 1) inside the inline GetVFontMaxHeight / GetVStringExtents with GFAYToVY 0x009E1DD0 / GFAXToVX
//   0x009E1DA0, CRenderManagerCore::SetVWindow 0x00A0B560, CWideString(const wchar_t*) 0x0099B6B0 / (const CWideString&) 0x0099B720 /
//   () 0x0099AED0 / ~ 0x0099B510 / GetAsCharArray 0x0099AF10 / Sprintf 0x0099BA70, CRenderManager2D::Draw2DText 0x009DD8F0 (eleven
//   calls), GFFloatToWideString 0x00486C40 (six; its result built in the by-value string slot).
// Globals: VIRTUAL_SCREEN_WIDTH 0x013961E8 / VIRTUAL_SCREEN_HEIGHT 0x013961F0 (established spellings), PRenderManager 0x013B8384
//   (?PRenderManager@@3PAVCRenderManager@@A: GFInitialise 0x004022B0 copies PDisplayManager->PRenderManager (Ego_r CDisplayManager
//   +0x08 scoped_ptr<CRenderManager>) into it, the pattern of its established neighbours PDisplayManager / PInputManager /
//   PFontManager; the four bound spellings g_pRenderManager2D / g_pDisplayCaps / g_RenderManagerCore_013b8384 /
//   g_FableDisplayPrimitive_013B8384 are stand-ins, NAME_FIXES), literals L"Freecam Info" / L"CameraPos" / L"CameraVec" / L"FOV" / L"%d".
// Inline helpers (retail expands them here): FableWin.pdb ?GFRoundFloat@@YIMMJ@Z (defined right after GFFloatToWideString in the
//   CPlayer TU: the multiplier loop `mov eax,places; dec; fmul 10.0f; jne`, folded for one place), ?GFIntToWideString@@YI?AVCWideString@@J@Z
//   (out of line elsewhere at 0x00405F00, landed: the same body).
// Shape: the line height is an unsigned long (the float conversion adds 2^32 when negative) whose conversion is reused from a
//   spill slot; the column reset is `pos.X = VIRTUAL_SCREEN_WIDTH * 0.25;` with the implicit double-to-float conversion (an
//   explicit (float) cast schedules the store before the colour-pointer push); GFRoundFloat's quotient is a named local
//   (`fild; fdiv st(1)` keeps the multiplier until after the double argument store).
// Headers: engine/CCamera.h and engine/C2DExtentsF.h are new; CPlayer.h FABLE_RETAIL_CCAMERA types the three cameras; CFontBank.h
//   FABLE_RETAIL_CFONTBANK is the polymorphic retail class; CRenderManager2D.h FABLE_RETAIL_CRENDERMANAGER2D declares Draw2DText.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCAMERA
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRENDERMANAGER2D
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/C2DExtentsF.h"
#include "engine/CFontBank.h"
#include "engine/CCamera.h"
#include "engine/CRenderManager.h"
#include "engine/CPlayer.h"

extern double VIRTUAL_SCREEN_WIDTH;                                 // 0x013961E8
extern double VIRTUAL_SCREEN_HEIGHT;                                // 0x013961F0
extern CRenderManager* PRenderManager;                              // 0x013B8384
CWideString __fastcall GFFloatToWideString(double value);           // 0x00486C40
inline CWideString __fastcall GFIntToWideString(long value)         // 0x00405F00 (out of line elsewhere)
{
    CWideString text;
    text.Sprintf(L"%d", value);
    return text;
}
inline float __fastcall GFRoundFloat(float value, long places)
{
    float multiplier = 1.0f;
    while (places--)
        multiplier *= 10.0f;
    float rounded = (float)(long)(value * multiplier);
    return rounded / multiplier;
}

void CPlayer::DrawFreeCamDebug(const C2DBoxF* box, const CFontBank& font) const
{
    float fov = GFRoundFloat(CurrentFreeCamera.GetHorizontalFOV() * 360.0f, 1);
    C2DVector pos((float)(VIRTUAL_SCREEN_WIDTH * 0.25), (float)(VIRTUAL_SCREEN_HEIGHT * 0.125));
    unsigned long lineHeight = (unsigned long)font.GetVFontMaxHeight();
    PRenderManager->SetVWindow(*box);
    CWideString title(L"Freecam Info");
    PRenderManager->Draw2DText(pos, title, font, CRGBColour(255, 255, 255, 255), 0);
    C2DExtentsF extents = font.GetVStringExtents(title);
    pos.Y += lineHeight * 2.0f;
    PRenderManager->Draw2DText(pos, L"CameraPos", font, CRGBColour(255, 255, 255, 255), 0);
    pos.X += extents.X;
    PRenderManager->Draw2DText(pos, GFFloatToWideString(GFRoundFloat(CurrentFreeCamera.PeekWorldPos().X, 2)), font, CRGBColour(255, 255, 255, 255), 0);
    pos.X += extents.X;
    PRenderManager->Draw2DText(pos, GFFloatToWideString(GFRoundFloat(CurrentFreeCamera.PeekWorldPos().Y, 2)), font, CRGBColour(255, 255, 255, 255), 0);
    pos.X += extents.X;
    PRenderManager->Draw2DText(pos, GFFloatToWideString(GFRoundFloat(CurrentFreeCamera.PeekWorldPos().Z, 2)), font, CRGBColour(255, 255, 255, 255), 0);
    pos.X = VIRTUAL_SCREEN_WIDTH * 0.25;
    pos.Y += lineHeight;
    PRenderManager->Draw2DText(pos, L"CameraVec", font, CRGBColour(255, 255, 255, 255), 0);
    pos.X += extents.X;
    PRenderManager->Draw2DText(pos, GFFloatToWideString(GFRoundFloat(CurrentFreeCamera.PeekViewDirection().X, 2)), font, CRGBColour(255, 255, 255, 255), 0);
    pos.X += extents.X;
    PRenderManager->Draw2DText(pos, GFFloatToWideString(GFRoundFloat(CurrentFreeCamera.PeekViewDirection().Y, 2)), font, CRGBColour(255, 255, 255, 255), 0);
    pos.X += extents.X;
    PRenderManager->Draw2DText(pos, GFFloatToWideString(GFRoundFloat(CurrentFreeCamera.PeekViewDirection().Z, 2)), font, CRGBColour(255, 255, 255, 255), 0);
    pos.X = VIRTUAL_SCREEN_WIDTH * 0.25;
    pos.Y += lineHeight;
    PRenderManager->Draw2DText(pos, L"FOV", font, CRGBColour(255, 255, 255, 255), 0);
    pos.X += extents.X;
    PRenderManager->Draw2DText(pos, GFIntToWideString((long)fov), font, CRGBColour(255, 255, 255, 255), 0);
}