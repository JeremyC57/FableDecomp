// CSubtitleRenderer::DrawButtonGraphics(const CEnginePrimitivePrepareInfo&, std::vector<CEnginePrimitiveHandle>&, const C2DVector&, bool) -- draws the text-box button graphics (pulsing confirm / cancel / third button sprites) into the persistent button primitives
// Grade: DIFFER(2662v2662) -- 14 hunks on TODAY's headers + this hdr/ (re-based acctB draft; CPLAYERGUIDEF_TEXTBOX -> today's CPLAYERGUIDEF_CLASS, CRGBCOLOUR_CTOR_INITIALISE -> today's CTOR4_INITIALISE): (1) retail tests DisplayFlags & 0x800 early and keeps the flags to the je at 0x0048EA81 after the box3 stores, with `mov [ebp+7Fh],0` / `mov [ebp+7Bh],0` (0x0048EA1D / 0x0048EA2A) interleaved; ours puts the layer if ahead of the bools/box3; (2) arm A stores TLX/BRX swapped v retail TLX,TLY,BRX,BRY with both offset addresses CSE'd (0x0048EA97..0x0048EABC). Earlier acctB residue note follows on the next lines.
// Leaf: ?DrawButtonGraphics@CSubtitleRenderer@@AAE_NABVCEnginePrimitivePrepareInfo@@AAV?$vector@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@ABVC2DVector@@_N@Z
// Retail 0x0048E78E, 2662 bytes, __thiscall `ret 10h`, #pragma optimize("s", on) alone (biased frame lea ebp,[esp-68h]).
// Callees: CFadeInFadeOutBase::Update 0x00434870, ::Open 0x00434840, ::IsOpen 0x0048DA30 (out of line), ::GetFadeInOutAlpha 0x004348D0,
// CPlayerGui::PeekGuiDef 0x00438570, CFontManager::GetFont 0x009E2C80 (+ CCountedPointer<CFontBank> dtor 0x004057A0),
// CSubtitleRenderer::GetTextWidthFromText 0x0048E062, GFVXToAX 0x009E1E10, CDisplayManager::GetRenderTargetDimensions 0x009BEDC0,
// CGraphicDataBank::GetGraphicMaxWidth (vtable +0x5C), CEnginePrimitive2DSprite ctors 0x00493AC2 / 0x0041BEB0, CIEngine::SetPersistentPrimitive (vtable +0x5C),
// CWideString::operator!= 0x0099B150, IsCutsceneSkippable 0x0048DE44, IsTextReadyForClickPast 0x0048E072, CPlayerManager::GetMainPlayer 0x00449970,
// NPlayerGui::CDrawQuestInfo::GetMiniGameType 0x00644D54, NPlayerGui::GFAnchorToTopRight 0x00643CA1. Inline: IsCutsceneActionModeEnabled, IsFlagSet (FableWin),
// CEnginePrimitiveHandle::Clear. Shape notes: pos.Y offsets as if/else arms of three += (sibling DrawButtonText 0x0048FAB6 shape; VC cross-jumps them
// into the lea select at 0x0048E86F); the two bare blocks (box1..return, box3..arms) are FITTED (frame 0x16C / tail temporary in box3's slot).
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_FORCEINLINE_RELEASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_FORCEINLINE_CLEAR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CIENGINE_CONVERTSCREENPOS
#define FABLE_RETAIL_CIENGINE_CONVERTWORLDPOS
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_VSTRINGWIDTH
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISACTIVE
#define FABLE_RETAIL_CFADEINFADEOUTBASE_FADE
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISOPEN_OUTOFLINE
#define FABLE_RETAIL_CSUBTITLERENDERER_BUTTONGRAPHICS
#define FABLE_RETAIL_CSUBTITLERENDERER_ISFLAGSET
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#define FABLE_RETAIL_CPLAYERGUI_GRAPHICBANK
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_CDRAWQUESTINFO_MINIGAMETYPE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CWORLD_UPDATEINPUT
#define FABLE_RETAIL_CPLAYER_RESPAWNDELAY
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/CFontBank.h"
#include "engine/CFontManager.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CFadeInFadeOutBase.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerGui.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"

// Free functions with no shared header (declare in the row; exact PDB spellings):
float __fastcall GFVXToAX(float x);                                      // 0x009E1E10 ?GFVXToAX@@YIMM@Z (Ego_r + FableWin)
namespace NPlayerGui {
C2DVector __fastcall GFAnchorToTopRight(const C2DVector& pos);           // 0x00643CA1 ?GFAnchorToTopRight@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z
}
extern CDisplayManager* GPDisplayManager;                                // 0x013B8390 ?GPDisplayManager@@3PAVCDisplayManager@@A (reloc_bindings)


#define BUTTON_SPRITE(name, box, layer, graphic, alpha) \
    CEnginePrimitive2DSprite name(0, box, layer, graphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, \
                                  alpha, 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f))

bool CSubtitleRenderer::DrawButtonGraphics(const CEnginePrimitivePrepareInfo& info, std::vector<CEnginePrimitiveHandle>& primitive_handles,
                                           const C2DVector& pos, bool in_text_box)
{
    const CInterpolationInfo::CInterpolationInfoSet& set = CMainGameComponent::Get()->PeekWorld().IsPaused() ? info.Interpolate.PausedCurrent
                                                                                                           : info.Interpolate.Current;
    ButtonPulseTimer.Update(set.GTPredictedTimeSinceLastRenderFrame);
    if (!ButtonPulseTimer.IsActive()) {
        if (IsCutsceneActionModeEnabled())
            ButtonPulseTimer.Open(0.3f, 0.3f, CRGBColour(0, 0, 0, 255));
        else
            ButtonPulseTimer.Open(0.6f, 0.6f, CRGBColour(0, 0, 0, 255));
    }
    else if (ButtonPulseTimer.IsOpen())
        ButtonPulseTimer.Close();

    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
    const CFontBank& font = *PFontManager->GetFont(def.TextBoxButtonTextFontName);

    C2DVector button1_pos(0.0f, pos.Y);
    C2DVector button2_pos(0.0f, pos.Y);
    C2DVector button3_pos(0.0f, pos.Y);
    if (in_text_box) {
        button1_pos.Y += def.TextBoxButtonInBoxOffsetY;
        button2_pos.Y += def.TextBoxButtonInBoxOffsetY;
        button3_pos.Y += def.TextBoxButtonInBoxOffsetY;
    } else {
        button1_pos.Y += def.TextBoxButtonOffsetY;
        button2_pos.Y += def.TextBoxButtonOffsetY;
        button3_pos.Y += def.TextBoxButtonOffsetY;
    }

    float button1_width = GFVXToAX(GetTextWidthFromText(Button1Text, font));
    float button2_width = GFVXToAX(GetTextWidthFromText(Button2Text, font));
    float button3_width = GFVXToAX(GetTextWidthFromText(Button3Text, font));

    if (IsCutsceneActionModeEnabled())
        button1_pos.X = (float)GPDisplayManager->GetRenderTargetDimensions().X + def.TextBoxTextAreaBRPos.X - 640.0f + 16.0f;
    else
        button1_pos.X = pos.X + 16.0f - button1_width;
    button2_pos.X = button1_pos.X - button2_width - def.TextBoxButtonGapWidth;
    button3_pos.X = button2_pos.X - button3_width - def.TextBoxButtonGapWidth;

    button1_pos += def.TextBoxButtonGraphicOffset;
    button2_pos += def.TextBoxButtonGraphicOffset;
    button3_pos += def.TextBoxButtonGraphicOffset;

    unsigned long graphic_max_width;
    if (IsCutsceneActionModeEnabled())
        graphic_max_width = CPlayerGui::Get().PeekGraphicBank().GetGraphicMaxWidth(def.TextBoxHugeMouseButtonGraphic);
    else
        graphic_max_width = CPlayerGui::Get().PeekGraphicBank().GetGraphicMaxWidth(def.TextBoxCancelGraphic);
    float graphic_width = (float)graphic_max_width;
    float pulse = ButtonPulseTimer.GetFadeInOutAlpha() * (2.0f / 15.0f);
    float half = pulse * graphic_width * 0.5f;
    button1_pos.X -= half;
    button1_pos.Y -= half;
    button2_pos.X -= half;
    button2_pos.Y -= half;
    button3_pos.X -= half;
    button3_pos.Y -= half;

    {
    C2DBoxF button1_box(button1_pos, C2DExtentsF((pulse + 1.0f) * graphic_width, (pulse + 1.0f) * graphic_width));
    C2DBoxF button2_box(button2_pos, C2DExtentsF((pulse + 1.0f) * graphic_width, (pulse + 1.0f) * graphic_width));
    EEngine2DLayer layer = ENGINE_2D_LAYER_2;
    if (IsFlagSet((ESubtitleFlag)0x800))
        layer = ENGINE_2D_LAYER_M2;
    bool drawn = false;
    bool button1_drawn = false;
    {
    C2DBoxF button3_box(button3_pos, C2DExtentsF((pulse + 1.0f) * graphic_width, (pulse + 1.0f) * graphic_width));

    if (IsCutsceneActionModeEnabled()) {
        button2_box.BRX += def.TextBoxHugeButtonOffset.X;
        button2_box.TLY += def.TextBoxHugeButtonOffset.Y;
        button2_box.TLX += def.TextBoxHugeButtonOffset.X;
        button2_box.BRY += def.TextBoxHugeButtonOffset.Y;
        C2DVector huge_pos(button2_box.TLX + half, button2_box.TLY + half);
        CEnginePrimitive2DSprite mouse_sprite(0, huge_pos, ENGINE_2D_LAYER_2, def.TextBoxHugeMouseButtonGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
                                              ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, Button2Alpha, 255, 255, 255, 2,
                                              C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine.SetPersistentPrimitive(primitive_handles[1], mouse_sprite, 0xc0);
        button2_box.TLX += def.TextBoxHugeButtonLeftOffset.X;
        button2_box.TLY += def.TextBoxHugeButtonLeftOffset.Y;
        button2_box.BRX += def.TextBoxHugeButtonLeftOffset.X;
        button2_box.BRY += def.TextBoxHugeButtonLeftOffset.Y;
        BUTTON_SPRITE(left_sprite, button2_box, ENGINE_2D_LAYER_1, def.TextBoxHugeMouseButtonLeftGraphic, Button2Alpha);
        info.Engine.SetPersistentPrimitive(primitive_handles[0], left_sprite, 0xc0);
        drawn = true;
        button1_drawn = true;
    } else if (IsFlagSet((ESubtitleFlag)4)) {
        BUTTON_SPRITE(sprite1, button1_box, layer, def.TextBoxCancelGraphic, Button1Alpha);
        info.Engine.SetPersistentPrimitive(primitive_handles[0], sprite1, 0xc0);
        BUTTON_SPRITE(sprite2, button2_box, layer, def.TextBoxConfirmGraphic, Button2Alpha);
        info.Engine.SetPersistentPrimitive(primitive_handles[1], sprite2, 0xc0);
        if (Button3Text != L"") {
            BUTTON_SPRITE(sprite3, button3_box, layer, def.TextBoxThirdGraphic, 255);
            info.Engine.SetPersistentPrimitive(primitive_handles[2], sprite3, 0xc0);
        }
        drawn = true;
        button1_drawn = true;
    } else if ((InCutscene || Button2TextOverridden) && !IsFlagSet((ESubtitleFlag)0x48)) {
        if (IsCutsceneSkippable() || Button2TextOverridden)
            drawn = true;
    } else if (IsFlagSet((ESubtitleFlag)2)) {
        if ((!InCutscene && IsTextReadyForClickPast()) || IsFlagSet((ESubtitleFlag)0x48)) {
            BUTTON_SPRITE(sprite, button1_box, layer, def.TextBoxConfirmGraphic, Button1Alpha);
            info.Engine.SetPersistentPrimitive(primitive_handles[0], sprite, 0xc0);
            drawn = true;
            button1_drawn = true;
            primitive_handles[1].Clear();
        }
    } else if (!InCutscene && CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPlayerRespawnDelay() == 0) {
        if (Button1Text != L"") {
            BUTTON_SPRITE(sprite, button1_box, layer, def.TextBoxCancelGraphic, Button1Alpha);
            info.Engine.SetPersistentPrimitive(primitive_handles[0], sprite, 0xc0);
            drawn = true;
            button1_drawn = true;
        }
        if (Button2Text != L"") {
            BUTTON_SPRITE(sprite, button2_box, layer, def.TextBoxConfirmGraphic, Button2Alpha);
            info.Engine.SetPersistentPrimitive(primitive_handles[1], sprite, 0xc0);
            drawn = true;
        }
        if (Button3Text != L"") {
            BUTTON_SPRITE(sprite, button3_box, layer, def.TextBoxThirdGraphic, 255);
            info.Engine.SetPersistentPrimitive(primitive_handles[2], sprite, 0xc0);
            drawn = true;
        }
    }
    }
    if (!button1_drawn && Button1TextOverridden) {
        button2_pos.X = button1_pos.X - GFVXToAX(GetTextWidthFromText(Button1OverrideText, font)) - def.TextBoxButtonGapWidth;
        if (CPlayerGui::Get().GetQuestInfo().GetMiniGameType() == MINIGAME_FISHING) {
            button2_pos = NPlayerGui::GFAnchorToTopRight(def.TextBoxFishingReelTextPos) + def.TextBoxFishingReelGraphicOffset;
            button2_pos.X -= half;
            button2_pos.Y -= half;
            float right = button2_pos.X + GFVXToAX(GetTextWidthFromText(Button1OverrideText, font));
            if ((float)GPDisplayManager->GetRenderTargetDimensions().X < right)
                button2_pos.X -= right - (float)GPDisplayManager->GetRenderTargetDimensions().X + 4.0f;
        }
        button2_box = C2DBoxF(button2_pos, C2DExtentsF((pulse + 1.0f) * graphic_width, (pulse + 1.0f) * graphic_width));
        BUTTON_SPRITE(sprite, button2_box, layer, def.TextBoxConfirmGraphic, Button1Alpha);
        info.Engine.SetPersistentPrimitive(primitive_handles[0], sprite, 0xc0);
        drawn = true;
    }
    return drawn;
    }
}
