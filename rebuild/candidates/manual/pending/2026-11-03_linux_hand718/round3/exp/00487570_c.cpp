// Leaf: ?DrawFreeCamDebug@CPlayer@@QBEXPBVC2DBoxF@@ABVCFontBank@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCAMERA
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRENDERMANAGER2D_TEXT
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
    return (float)(long)(value * multiplier) / multiplier;
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
    pos.X = (float)(VIRTUAL_SCREEN_WIDTH * 0.25);
    pos.Y += lineHeight;
    PRenderManager->Draw2DText(pos, L"CameraVec", font, CRGBColour(255, 255, 255, 255), 0);
    pos.X += extents.X;
    PRenderManager->Draw2DText(pos, GFFloatToWideString(GFRoundFloat(CurrentFreeCamera.PeekViewDirection().X, 2)), font, CRGBColour(255, 255, 255, 255), 0);
    pos.X += extents.X;
    PRenderManager->Draw2DText(pos, GFFloatToWideString(GFRoundFloat(CurrentFreeCamera.PeekViewDirection().Y, 2)), font, CRGBColour(255, 255, 255, 255), 0);
    pos.X += extents.X;
    PRenderManager->Draw2DText(pos, GFFloatToWideString(GFRoundFloat(CurrentFreeCamera.PeekViewDirection().Z, 2)), font, CRGBColour(255, 255, 255, 255), 0);
    pos.X = (float)(VIRTUAL_SCREEN_WIDTH * 0.25);
    pos.Y += lineHeight;
    PRenderManager->Draw2DText(pos, L"FOV", font, CRGBColour(255, 255, 255, 255), 0);
    pos.X += extents.X;
    PRenderManager->Draw2DText(pos, GFIntToWideString((long)fov), font, CRGBColour(255, 255, 255, 255), 0);
}
