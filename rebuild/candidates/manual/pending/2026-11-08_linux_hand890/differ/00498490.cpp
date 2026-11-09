// CProgressDisplay::DrawRetailDisplay(const CCharString& text_tag, float progress, bool use_progress) -- draw the retail loading / auto-save screen: advance the stage count (or follow the installer's progress), pick the next loading picture half way, draw the loading picture or the six auto-save backdrop tiles, the progress bar, the black screen, the guild seal animation and the tip / save text
// Grade: DIFFER(5101v5101) (probe r3_v7, 7 grades) -- size equal, strict 84.1% / loose 99.9%. Residue: (1) frame slot packing (same frame 0x5C; formattedWindow joins slot A 0x10 so display / scaleX / text / fraction / the 0x30-0x38 groups sit 8 bytes higher than retail; c2 packer: items by size asc + weight desc, each joins the most recent slot it fits (size <= 2x slot, no interference), work/s2/e10.order); (2) `mov ecx,edi` one slot early at 0x00499094.
// Next: find why retail's tile-1 temporary T11920 (`GFAYToVY(256.0f) * scaleY`) cannot join slot A (total, tag) and opens slot 0x38 (work/s2/e10.order lists every item with retail's slot); then the seal block's `this` load order.
// Leaf: ?DrawRetailDisplay@CProgressDisplay@@AAEXABVCCharString@@M_N@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CCHARSTRING_ENDSWITH
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DTEXTUREBOX
#define FABLE_RETAIL_CRENDERMANAGER2D_LOADINGSCREEN
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_GRAPHICBANK
#define FABLE_RETAIL_CPLAYERGUIDEF_PROGRESS
#define FABLE_RETAIL_SINGLETON_ISSETUP
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK_INITIALISED
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER_AUTOSAVENAME
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING
#include "fable_string.h"
#include "fable_user_profile.h"
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CPixelFormat.h"
#include "engine/CTexture.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CFontBank.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CNewFrontendGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CProgressDisplay.h"

double __fastcall GFGetTime();                                           // 0x009E1BC0 ?GFGetTime@@YINXZ
float __fastcall GFRoundVXToNearestPixel(double x);                      // 0x009E1D00 ?GFRoundVXToNearestPixel@@YIMN@Z
float __fastcall GFRoundVYToNearestPixel(double y);                      // 0x009E1D50 ?GFRoundVYToNearestPixel@@YIMN@Z
void __fastcall GFABoxToVBox(const C2DBoxF& abox, C2DBoxF& vbox);        // 0x009E2020 ?GFABoxToVBox@@YIXABVC2DBoxF@@AAV1@@Z
void __fastcall WordWrap_SetCallback(unsigned int (__fastcall* char_width)(wchar_t), unsigned int (__fastcall* line_height)());   // 0x00A0FD90
extern double VIRTUAL_SCREEN_WIDTH;                                      // 0x013961E8
extern double VIRTUAL_SCREEN_HEIGHT;                                     // 0x013961F0
extern CDisplayManager* GPDisplayManager;                                // 0x013B8390
extern bool g_bUseCompiledWorldFiles;                                    // 0x013B8618
extern bool g_bUseCompiledGlobalThings;                                  // 0x013B8609
extern bool g_WADBuildEnabled;                                           // 0x01375459
extern bool g_bAutoSaveProgressShown;                                    // 0x013B89C1
extern bool g_bSaveLoadFailed;                                           // 0x013B89C0
extern bool g_bProgressDisplaySuppressed;                                // 0x013B89D3
extern double g_AutoSaveProgressEndTime;                                 // 0x013B89C8
extern CWideString GResetProgressTipText;                                // 0x013B89D8
extern CWideString GResetProgressTipTitleText;                           // 0x013B89DC

template <class T> inline const T& __fastcall GFLimitReturn(const T& value, const T& low, const T& high)
{
    if (value < low)
        return low;
    if (value > high)
        return high;
    return value;
}

const NGameText::CDataBank* CProgressDisplay::GetPTextBank() const
{
    if (CMainGameComponent::Get() && CMainGameComponent::Get()->IsTextBankInitialised())
        return &CMainGameComponent::Get()->GetTextBank();
    if (CNewFrontendGameComponent::Get() && CNewFrontendGameComponent::Get()->IsTextBankInitialised())
        return &CNewFrontendGameComponent::Get()->GetTextBank();
    return 0;
}

void CProgressDisplay::DrawRetailDisplay(const CCharString& text_tag, float progress, bool use_progress)
{
    CDisplayManager& display = *GFGetSystemManager()->GetDisplayManager();
    CRenderManager& renderer = display.GetRenderManager();
    if (FullScreen) {
        long total = 130;
        if (!g_bUseCompiledWorldFiles)
            total = 465;
        if (!g_bUseCompiledGlobalThings)
            total += 347;
        if (g_WADBuildEnabled)
            total += 1788;
        if (g_bAutoSaveProgressShown)
            total = 300;
        ++StageCount;
        if (g_bAutoSaveProgressShown)
            StageCount = (long)(300.0 - (g_AutoSaveProgressEndTime - GFGetTime()) * 100.0);
        bool restarted = false;
        float fraction = (float)GFLimitReturn(StageCount, 0L, total) / total;
        if (use_progress) {
            fraction = progress;
            if (progress >= 1.0f) {
                fraction = 1.0f;
                if (!FileInstallerFinished) {
                    FileInstallerFinished = true;
                    ForFileInstaller = false;
                    StageCount = 0;
                    fraction = 0.0f;
                    restarted = true;
                }
            }
        }
        if (!g_bAutoSaveProgressShown && (StageCount == total / 2 || restarted)) {
            ++LoadingScreenIndex;
            if (LoadingScreenIndex == (long)LoadingScreenTextures.size())
                LoadingScreenIndex = 0;
        }
        if (g_bAutoSaveProgressShown) {
            if (CPlayerGui::IsSingletonSetup()) {
                CPlayerGui::Get().ChangeGraphicsBankScene();
                const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
                float scaleX = GPDisplayManager->GetRenderTargetDimensions().X * (1.0f / 640.0f);
                float scaleY = GPDisplayManager->GetRenderTargetDimensions().Y * (1.0f / 480.0f);
                C2DBoxF box;
                box = C2DBoxF(C2DVector(GFAXToVX(0.0f), GFAYToVY(0.0f)), C2DVector(GFAXToVX(256.0f) * scaleX, GFAYToVY(256.0f) * scaleY));
                renderer.Draw2DScaledGraphic(box, CPlayerGui::Get().PeekGraphicBank(), def.LoadProgressBackdropGraphics[0], 0, 0);
                box = C2DBoxF(C2DVector(box.BRX, box.TLY), C2DVector(box.BRX + GFAXToVX(256.0f) * scaleX, GFAYToVY(256.0f) * scaleY));
                renderer.Draw2DScaledGraphic(box, CPlayerGui::Get().PeekGraphicBank(), def.LoadProgressBackdropGraphics[1], 0, 0);
                box = C2DBoxF(C2DVector(box.BRX, box.TLY), C2DVector(box.BRX + GFAXToVX(128.0f) * scaleX, GFAYToVY(256.0f) * scaleY));
                renderer.Draw2DScaledGraphic(box, CPlayerGui::Get().PeekGraphicBank(), def.LoadProgressBackdropGraphics[2], 0, 0);
                box = C2DBoxF(C2DVector(GFAXToVX(0.0f), box.BRY), C2DVector(GFAXToVX(256.0f) * scaleX, box.BRY + GFAYToVY(256.0f) * scaleY));
                renderer.Draw2DScaledGraphic(box, CPlayerGui::Get().PeekGraphicBank(), def.LoadProgressBackdropGraphics[3], 0, 0);
                box = C2DBoxF(C2DVector(box.BRX, box.TLY), C2DVector(box.BRX + GFAXToVX(256.0f) * scaleX, box.TLY + GFAYToVY(256.0f) * scaleY));
                renderer.Draw2DScaledGraphic(box, CPlayerGui::Get().PeekGraphicBank(), def.LoadProgressBackdropGraphics[4], 0, 0);
                box = C2DBoxF(C2DVector(box.BRX, box.TLY), C2DVector(box.BRX + GFAXToVX(128.0f) * scaleX, box.TLY + GFAYToVY(256.0f) * scaleY));
                renderer.Draw2DScaledGraphic(box, CPlayerGui::Get().PeekGraphicBank(), def.LoadProgressBackdropGraphics[5], 0, 0);
            }
        } else {
            renderer.Draw2DTexture(C2DBoxF(0.0f, 0.0f, (float)VIRTUAL_SCREEN_WIDTH, (float)VIRTUAL_SCREEN_HEIGHT),
                                   CRGBColour(255, 255, 255, 255), LoadingScreenTextures[LoadingScreenIndex], 0);
        }
        if ((!ProgressTextures.empty() || g_bAutoSaveProgressShown) && !g_bSaveLoadFailed) {
            float renderHeight = (float)display.GetRenderTargetDimensions().Y;
            C2DVector barPosition;
            barPosition.X = GFAXToVX((float)display.GetRenderTargetDimensions().X) * 0.5f - GFAXToVX(96.0f);
            barPosition.Y = GFAYToVY(renderHeight + 380.0f - 480.0f);
            C2DBoxF box(barPosition, C2DExtentsF(GFAXToVX(64.0f), GFAYToVY(32.0f)));
            box.TLX = GFRoundVXToNearestPixel(box.TLX);
            box.TLY = GFRoundVYToNearestPixel(box.TLY);
            box.BRX = GFRoundVXToNearestPixel(box.BRX);
            box.BRY = GFRoundVYToNearestPixel(box.BRY);
            if (g_bAutoSaveProgressShown) {
                const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
                renderer.Draw2DScaledGraphic(box, CPlayerGui::Get().PeekGraphicBank(), def.LoadProgressBarLeftGraphic, 0, 1);
            } else
                renderer.Draw2DTexture(box, CRGBColour(255, 255, 255, 255), ProgressTextures[1], 1);
            C2DVector position(barPosition.X + GFAXToVX(64.0f), barPosition.Y);
            box = C2DBoxF(position, C2DExtentsF(GFAXToVX(64.0f), GFAYToVY(32.0f)));
            box.TLX = GFRoundVXToNearestPixel(box.TLX);
            box.TLY = GFRoundVYToNearestPixel(box.TLY);
            box.BRX = GFRoundVXToNearestPixel(box.BRX);
            box.BRY = GFRoundVYToNearestPixel(box.BRY);
            if (g_bAutoSaveProgressShown) {
                const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
                renderer.Draw2DScaledGraphic(box, CPlayerGui::Get().PeekGraphicBank(), def.LoadProgressBarMidGraphic, 0, 1);
            } else
                renderer.Draw2DTexture(box, CRGBColour(255, 255, 255, 255), ProgressTextures[2], 1);
            position.X += GFAXToVX(64.0f);
            box = C2DBoxF(position, C2DExtentsF(GFAXToVX(64.0f), GFAYToVY(32.0f)));
            box.TLX = GFRoundVXToNearestPixel(box.TLX);
            box.TLY = GFRoundVYToNearestPixel(box.TLY);
            box.BRX = GFRoundVXToNearestPixel(box.BRX);
            box.BRY = GFRoundVYToNearestPixel(box.BRY);
            if (g_bAutoSaveProgressShown) {
                const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
                renderer.Draw2DScaledGraphic(box, CPlayerGui::Get().PeekGraphicBank(), def.LoadProgressBarRightGraphic, 0, 1);
            } else
                renderer.Draw2DTexture(box, CRGBColour(255, 255, 255, 255), ProgressTextures[3], 1);
            float fillWidth = GFAXToVX(146.0f) * fraction;
            if (fillWidth > 0.0f) {
                C2DVector fillPosition = barPosition + C2DVector(GFAXToVX(23.0f), GFAYToVY(5.0f));
                box = C2DBoxF(fillPosition, C2DExtentsF(fillWidth, GFAYToVY(16.0f)));
                box.TLX = GFRoundVXToNearestPixel(box.TLX);
                box.TLY = GFRoundVYToNearestPixel(box.TLY);
                box.BRX = GFRoundVXToNearestPixel(box.BRX);
                box.BRY = GFRoundVYToNearestPixel(box.BRY);
                if (g_bAutoSaveProgressShown) {
                    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
                    renderer.Draw2DScaledGraphic(box, CPlayerGui::Get().PeekGraphicBank(), def.LoadProgressBarInnerGraphic, 0, 0);
                } else
                    renderer.Draw2DTexture(box, CRGBColour(255, 255, 255, 255), ProgressTextures[4], 1);
            }
            if (!g_bAutoSaveProgressShown) {
                position = C2DVector(GFAXToVX(display.GetRenderTargetDimensions().X * 0.5f - 129.0f), GFAYToVY(renderHeight + 315.0f - 480.0f));
                box = C2DBoxF(position, C2DExtentsF(GFAXToVX(256.0f), GFAYToVY(128.0f)));
                box.TLX = GFRoundVXToNearestPixel(box.TLX);
                box.TLY = GFRoundVYToNearestPixel(box.TLY);
                box.BRX = GFRoundVXToNearestPixel(box.BRX);
                box.BRY = GFRoundVYToNearestPixel(box.BRY);
                renderer.Draw2DTexture(box, CRGBColour(255, 255, 255, 255), ProgressTextures[5], 1);
            }
        }
    }
    if (BlackScreen) {
        C2DBoxF screen(0.0f, 0.0f, (float)VIRTUAL_SCREEN_WIDTH, (float)VIRTUAL_SCREEN_HEIGHT);
        renderer.Draw2DBox(screen, CRGBColour(0, 0, 0, 255), 0);
    }
    if (!g_bAutoSaveProgressShown && !g_bSaveLoadFailed && !ForFrontEnd) {
        float sealX = GFAXToVX((float)display.GetRenderTargetDimensions().X) * 0.5f - GFAXToVX(32.0f) - 4.0f;
        C2DVector sealPosition(sealX, GFAYToVY(display.GetRenderTargetDimensions().Y + 310.0f - 480.0f));
        if (FullScreen) {
            if ((unsigned long)GuildSealFrameIndex < GuildSealTextures.size())
                renderer.Draw2DTexture(C2DBoxF(sealPosition, C2DExtentsF(GFAXToVX(64.0f), GFAYToVY(64.0f))), CRGBColour(255, 255, 255, 255),
                                       GuildSealTextures[GuildSealFrameIndex], 1);
            if (!GuildSealTextures.empty()) {
                ++GuildSealFrameIndex;
                GuildSealFrameIndex %= (long)GuildSealTextures.size();
            }
        } else if (CPlayerGui::IsSingletonSetup()) {
            const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
            if ((unsigned long)GuildSealFrameIndex < def.RegionChangeGuildSealGraphics.size()) {
                float right = display.GetRenderTargetDimensions().X - 112.0f;
                float bottom = display.GetRenderTargetDimensions().Y + 380.0f - 480.0f;
                sealPosition = C2DVector(GFAXToVX(right), GFAYToVY(bottom));
                renderer.Draw2DBox(C2DBoxF(sealPosition, C2DExtentsF(GFAXToVX(64.0f), GFAYToVY(64.0f))), CRGBColour(0, 0, 0, 255), 0);
                sealPosition = C2DVector(GFAXToVX(right), GFAYToVY(bottom));
                renderer.Draw2DGraphic(sealPosition, CPlayerGui::Get().PeekGraphicBank(), def.RegionChangeGuildSealGraphics[GuildSealFrameIndex], 0, 1, 0xFFFFFFFF);
            }
            if (!def.RegionChangeGuildSealGraphics.empty()) {
                ++GuildSealFrameIndex;
                GuildSealFrameIndex %= (long)def.RegionChangeGuildSealGraphics.size();
            }
        }
        if (LastTextDisplayed != "")
            GFGetTime();
        LastProgressUpdate = GFGetTime();
    }
    if (FullScreen) {
        CWideString text(GResetProgressTipText);
        CWideString title(GResetProgressTipTitleText);
        if (g_bAutoSaveProgressShown) {
            text = L"";
            title = L"";
        }
        const NGameText::CDataBank* textBank = GetPTextBank();
        if (textBank) {
            if (DisplayText) {
                if (!DisplayResetProgressTipText) {
                    text = textBank->GetTextBySymbol(TextTag);
                    CCharString tag(TextTag);
                    if (tag.EndsWith("0"))
                        tag = tag.GetSubString(0, tag.GetLength() - 3);
                    if (tag.EndsWith("_PC"))
                        tag = tag.GetSubString(0, tag.GetLength() - 3);
                    title = textBank->GetTextBySymbol(tag + "_TITLE");
                }
                if (g_bAutoSaveProgressShown) {
                    text = textBank->GetTextBySymbol("TEXT_GUI_SAVE_SAVING_EXTENDED_10");
                    NGameText::ConstructString(text, CUserProfileManager::Get().GetAutoSaveDisplayName());
                    title = L"";
                }
            }
            if (g_bProgressDisplaySuppressed) {
                text = L"A - " + textBank->GetTextBySymbol("TEXT_GUI_INVENTORY_BUTTON_TEXT_EXIT");
                title = textBank->GetTextBySymbol("TEXT_GUI_MENU_INVALID_SAVE");
                NGameText::ConstructString(title, CUserProfileManager::Get().GetAutoSaveDisplayName());
            }
        }
        if (text != L"") {
            WordWrap_SetCallback(GetCharWidthW, 0);
            float left = display.GetRenderTargetDimensions().X * 0.125f;
            float scale = display.GetRenderTargetDimensions().Y * (1.0f / 480.0f);
            C2DBoxF vbox;
            GFABoxToVBox(C2DBoxF(C2DVector(left, scale * 100.0f), C2DVector(display.GetRenderTargetDimensions().X - left, scale * 300.0f)), vbox);
            CWideString formatted;
            C2DBoxF formattedWindow;
            FormatTextForVWindow(vbox, *PFont, text, formatted, formattedWindow);
            vbox.TLY -= GFAYToVY(10.0f);
            vbox.BRY -= GFAYToVY(10.0f);
            DisplayCenteredText(*PFont, formatted, vbox);
            if (title != L"") {
                C2DExtentsF extents = PFont->GetVStringExtents(formatted);
                vbox.TLY -= extents.Y;
                vbox.BRY -= extents.Y;
                DisplayCenteredText(*PFont, title, vbox);
            }
        }
    }
}
