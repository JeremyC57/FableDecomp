// NPlayerGui::CDrawMiniMap::Draw(const CEnginePrimitivePrepareInfo& interpolation, bool drawing_hud) -- skip while a live hero's region display covers an active map in letterbox mode; otherwise mark the drawer drawing, step the screen-position fade, size the centred map box (0.4 of the render width) and blend it toward the anchored ScreenBox, clamp the fade alpha, draw the map; on a region change draw the six-tile frame and the two side borders, otherwise label every visible region exit
// Grade: DIFFER(3464v3464) -- wfE9-9: frame = retail (all slots), equal length/calls; only residue = SIB base/index order of &RegionExits[i].second at 0x006493B5 / 0x0064941B
// Leaf: ?Draw@CDrawMiniMap@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00648820, 3464 bytes, `ret 8`, size-optimised NPlayerGui TU (`lea ebp,[esp-70h]` frame).
// Name: Ego_r.pdb / FableWin.pdb ?Draw@CDrawMiniMap@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@_N@Z; parameter / local names from the
//   Anniversary Xbox 360 PDB (interpolation, drawing_hud, map_box, text); tile / border locals descriptive.
// EVIDENCED: clip-box sprite ctor body visible (overlay opt-in BOX_CTOR_BODY, proven = 0x0041BEB0 with C2DBOXF_COPY); same-TU
//   NPlayerGui::GFToScreenScaled defined above the row (landed 0x00643BFC logic on shared headers; the retail TU also holds the C2DBoxF
//   COMDATs 0x00643930..0x00643970) -- moves the main window to its retail slot -4; the six tiles via C2DBoxF::SetTLPos / SetWidth /
//   SetHeight (FableWin.pdb emits exactly these three inline bodies right after CDrawMiniMap::Draw = first used there; overlay opt-in
//   C2DBOXF_SETTERS): next tile at C2DVector(tile.TLX + tile.GetWidth(), tile.TLY) (retail fld BRX ; fsub TLX ; fadd TLX, 0x00648BE7),
//   row 2 at C2DVector(RegionChangeTLOffset.X, tile.BRY) (0x00648DF3); borders = a C2DVector copy of the offset + C2DExtentsF(width,
//   height) through the inline (top_left, extents) ctor (retail keeps left_pos.Y in [ebp+40h], 0x00649163; still called at ScreenBox
//   0x006489B5, expanded here with no device), then `left = GFToScreenScaled(left)` (movsd copy, 0x0064918E); border height evaluates
//   GetRenderTargetDimensions before the bank call (0x0064919F); the centred map box built as C2DBoxF(centre_tl, C2DExtentsF(size, size))
//   and copied (its BR words at [ebp+50h] share the tile slot, as retail). Frame 0x334 and every slot = retail (tools/pkfull.py model).
//   Wide literals L"" go to CWideString::operator==(const wchar_t*) (header signature; wchar_t is unsigned short under VC7.1).
//   Names: phero / ptc_region_display / map_box / text / text_pos from the Anniversary PDB (S_REGREL32 of this function); centred / tile /
//   left / right / *_pos / *_size / symbol / name / scale / text_alpha descriptive.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_CPLAYERGUI_DRAWTEXT
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_ISLETTERBOXMODE
#define FABLE_RETAIL_CDISPLAYENGINE_SCREENFADEOUT
#define FABLE_RETAIL_CDISPLAYENGINE_REGIONLOADFADE
#define FABLE_RETAIL_CDISPLAYENGINE_GETMAINWINDOW
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISACTIVE
#define FABLE_RETAIL_CFADEINFADEOUTBASE_DECLS
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISCLOSING
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISOPENING
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_NPLAYERGUI_CDRAWMINIMAP_TYPED
#define FABLE_RETAIL_NPLAYERGUI_ANCHORS
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREENSCALED
#define FABLE_RETAIL_CMINIMAPDISPLAY
#define FABLE_RETAIL_CMINIMAPDISPLAY_DRAWAPI
#define FABLE_RETAIL_CTCREGIONDISPLAY
#define FABLE_RETAIL_CTCREGIONDISPLAY_ISDRAWING
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_BOX_CTOR_BODY
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_SETTERS
#define FABLE_RETAIL_C2DBOXF_EXTENTS_DECL
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_STRING_HELPERS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CWORLD_UPDATEINPUT
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DExtentsI.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/ThingAlive.h"
#include "engine/CTCRegionDisplay.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/CFadeInFadeOutBase.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CFontBank.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/NPlayerGui_Graphics.h"
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/CMiniMapDisplay.h"
#include "engine/CDrawMiniMap.h"

extern CDisplayManager* GPDisplayManager;                    // 0x013B8390

namespace NPlayerGui {

// Same-TU callee (landed row NPlayerGui_GFToScreenScaled_00643bfc.cpp, retail 0x00643BFC; called, not expanded): its visible body
// tells c2 that the call keeps no address of the box arguments.
C2DBoxF FABLE_FASTCALL GFToScreenScaled(const C2DBoxF& box)
{
    return C2DBoxF((float)GPDisplayManager->GetRenderTargetDimensions().X * box.TLX * (1.0f / 640.0f),
                   (float)GPDisplayManager->GetRenderTargetDimensions().Y * box.TLY * (1.0f / 480.0f),
                   (float)GPDisplayManager->GetRenderTargetDimensions().X * box.BRX * (1.0f / 640.0f),
                   (float)GPDisplayManager->GetRenderTargetDimensions().Y * box.BRY * (1.0f / 480.0f));
}

void CDrawMiniMap::Draw(const CEnginePrimitivePrepareInfo& interpolation, bool drawing_hud)
{
    if (Active && !drawing_hud && Component->GetDisplayEngine().IsLetterBoxMode()) {
        const CThingPlayerCreature* phero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
        if (IS_THING_ALIVE(phero)) {
            const CTCRegionDisplay* ptc_region_display;
            phero->PeekTC(TCI_REGION_DISPLAY, &ptc_region_display);
            if (!ptc_region_display->IsDrawing())
                return;
        }
    }
    if (!Active && !RegionChange && !FadeInOut.IsActive())
        return;
    SetAsDrawing(true);
    if (!World->IsPaused() && !Component->GetDisplayEngine().IsScreenFadeOutClosing())
        ScreenPosFadeInOut.Update(interpolation.Interpolate.Current.GTPredictedTimeSinceLastRenderFrame);

    C2DBoxF screen = Component->GetDisplayEngine().GetMainWindow();
    float size = GPDisplayManager->GetRenderTargetDimensions().X * 0.4f;
    C2DVector centre_tl((screen.GetWidth() - size) * 0.5f, (screen.GetHeight() - size) * 0.5f);
    C2DBoxF centred(centre_tl, C2DExtentsF(size, size));
    C2DBoxF map_box = centred;
    if (!RegionChange) {
        const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
        ScreenBox = C2DBoxF(GFAnchorToTopRight(def.MiniMapScreenOrigin), C2DExtentsF(def.MiniMapScreenSize.X, def.MiniMapScreenSize.Y));
        map_box = ScreenBox;
        if (ScreenPosFadeInOut.IsOpen() || ScreenPosFadeInOut.IsOpening() || ScreenPosFadeInOut.IsClosing()) {
            map_box.TLX += ScreenPosFadeInOut.GetFadeInOutAlpha() * (centred.TLX - ScreenBox.TLX);
            map_box.TLY += ScreenPosFadeInOut.GetFadeInOutAlpha() * (centred.TLY - ScreenBox.TLY);
            map_box.BRX += ScreenPosFadeInOut.GetFadeInOutAlpha() * (centred.BRX - ScreenBox.BRX);
            map_box.BRY += ScreenPosFadeInOut.GetFadeInOutAlpha() * (centred.BRY - ScreenBox.BRY);
        }
    }

    float fade = FadeInOut.GetFadeInOutAlpha() * 255.0f;
    if (fade > 255.0f)
        fade = 255.0f;
    unsigned char alpha = (unsigned char)fade;
    if (alpha > Alpha)
        alpha = Alpha;
    unsigned char map_alpha = ScaleAlpha(alpha);
    if (RegionChange)
        map_alpha = alpha;
    MiniMap.DrawPreparePrimitivesForEngine(interpolation, map_box, map_alpha, RegionChange);

    if (RegionChange) {
        const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
        C2DBoxF tile;
        tile.SetTLPos(def.RegionChangeTLOffset);
        tile.SetWidth((float)(GPDisplayManager->GetRenderTargetDimensions().X * Bank->GetGraphicMaxWidth(def.RegionChangeTLGraphic)) * (1.0f / 640.0f));
        tile.SetHeight((float)(Bank->GetGraphicMaxHeight(def.RegionChangeTLGraphic) * GPDisplayManager->GetRenderTargetDimensions().Y) * (1.0f / 480.0f));
        CEnginePrimitive2DSprite top_left(0, tile, ENGINE_2D_LAYER_2, def.RegionChangeTLGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2,
            C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        interpolation.Engine.SetPersistentPrimitive(PrimitiveHandles[0], top_left, 0xc0);

        tile.SetTLPos(C2DVector(tile.TLX + tile.GetWidth(), tile.TLY));
        tile.SetWidth((float)(GPDisplayManager->GetRenderTargetDimensions().X * Bank->GetGraphicMaxWidth(def.RegionChangeTMGraphic)) * (1.0f / 640.0f));
        tile.SetHeight((float)(Bank->GetGraphicMaxHeight(def.RegionChangeTMGraphic) * GPDisplayManager->GetRenderTargetDimensions().Y) * (1.0f / 480.0f));
        CEnginePrimitive2DSprite top_middle(0, tile, ENGINE_2D_LAYER_2, def.RegionChangeTMGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2,
            C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        interpolation.Engine.SetPersistentPrimitive(PrimitiveHandles[1], top_middle, 0xc0);

        tile.SetTLPos(C2DVector(tile.TLX + tile.GetWidth(), tile.TLY));
        tile.SetWidth((float)(GPDisplayManager->GetRenderTargetDimensions().X * Bank->GetGraphicMaxWidth(def.RegionChangeTRGraphic)) * (1.0f / 640.0f));
        tile.SetHeight((float)(Bank->GetGraphicMaxHeight(def.RegionChangeTRGraphic) * GPDisplayManager->GetRenderTargetDimensions().Y) * (1.0f / 480.0f));
        CEnginePrimitive2DSprite top_right(0, tile, ENGINE_2D_LAYER_2, def.RegionChangeTRGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2,
            C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        interpolation.Engine.SetPersistentPrimitive(PrimitiveHandles[2], top_right, 0xc0);

        tile.SetTLPos(C2DVector(def.RegionChangeTLOffset.X, tile.BRY));
        tile.SetWidth((float)(GPDisplayManager->GetRenderTargetDimensions().X * Bank->GetGraphicMaxWidth(def.RegionChangeBLGraphic)) * (1.0f / 640.0f));
        tile.SetHeight((float)(Bank->GetGraphicMaxHeight(def.RegionChangeBLGraphic) * GPDisplayManager->GetRenderTargetDimensions().Y) * (1.0f / 480.0f));
        CEnginePrimitive2DSprite bottom_left(0, tile, ENGINE_2D_LAYER_2, def.RegionChangeBLGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2,
            C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        interpolation.Engine.SetPersistentPrimitive(PrimitiveHandles[3], bottom_left, 0xc0);

        tile.SetTLPos(C2DVector(tile.TLX + tile.GetWidth(), tile.TLY));
        tile.SetWidth((float)(GPDisplayManager->GetRenderTargetDimensions().X * Bank->GetGraphicMaxWidth(def.RegionChangeBMGraphic)) * (1.0f / 640.0f));
        tile.SetHeight((float)(Bank->GetGraphicMaxHeight(def.RegionChangeBMGraphic) * GPDisplayManager->GetRenderTargetDimensions().Y) * (1.0f / 480.0f));
        CEnginePrimitive2DSprite bottom_middle(0, tile, ENGINE_2D_LAYER_2, def.RegionChangeBMGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2,
            C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        interpolation.Engine.SetPersistentPrimitive(PrimitiveHandles[4], bottom_middle, 0xc0);

        tile.SetTLPos(C2DVector(tile.TLX + tile.GetWidth(), tile.TLY));
        tile.SetWidth((float)(GPDisplayManager->GetRenderTargetDimensions().X * Bank->GetGraphicMaxWidth(def.RegionChangeBRGraphic)) * (1.0f / 640.0f));
        tile.SetHeight((float)(Bank->GetGraphicMaxHeight(def.RegionChangeBRGraphic) * GPDisplayManager->GetRenderTargetDimensions().Y) * (1.0f / 480.0f));
        CEnginePrimitive2DSprite bottom_right(0, tile, ENGINE_2D_LAYER_2, def.RegionChangeBRGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2,
            C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        interpolation.Engine.SetPersistentPrimitive(PrimitiveHandles[5], bottom_right, 0xc0);

        C2DExtentsF left_size((float)Bank->GetGraphicMaxWidth(def.RegionChangeBorderLeftGraphic), (float)Bank->GetGraphicMaxWidth(def.RegionChangeBorderRightGraphic));
        C2DVector left_pos = def.RegionChangeBorderLeftOffset;
        C2DBoxF left(left_pos, left_size);
        left = GFToScreenScaled(left);
        float height = (float)Bank->GetGraphicMaxHeight(def.RegionChangeBorderLeftGraphic) * (GPDisplayManager->GetRenderTargetDimensions().Y * (1.0f / 480.0f));
        left.TLY = (GPDisplayManager->GetRenderTargetDimensions().Y - height) * 0.5f;
        left.BRY = left.TLY + height;
        CEnginePrimitive2DSprite left_border(0, left, ENGINE_2D_LAYER_1, def.RegionChangeBorderLeftGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2,
            C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        interpolation.Engine.SetPersistentPrimitive(PrimitiveHandles[6], left_border, 0xc0);

        C2DExtentsF right_size((float)Bank->GetGraphicMaxWidth(def.RegionChangeBorderRightGraphic), (float)Bank->GetGraphicMaxHeight(def.RegionChangeBorderRightGraphic));
        C2DVector right_pos = def.RegionChangeBorderRightOffset;
        C2DBoxF right(right_pos, right_size);
        right = GFToScreenScaled(right);
        right.TLY = left.TLY;
        right.BRY = left.BRY;
        CEnginePrimitive2DSprite right_border(0, right, ENGINE_2D_LAYER_1, def.RegionChangeBorderRightGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2,
            C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        interpolation.Engine.SetPersistentPrimitive(PrimitiveHandles[7], right_border, 0xc0);
    }

    if (!RegionChange && ScreenPosFadeInOut.IsActive()) {
        const CFontBank& font = Component->PeekCutSceneMainFont();
        for (unsigned long i = 0; i < RegionExits.size(); ++i) {
            const CCharString& symbol = RegionExits[i].second;
            CWideString text = Component->GetTextBank().GetTextBySymbol(symbol);
            if (text == L"")
                text = symbol.ToWideString();
            const CCharString& name = RegionExits[i].second;
            if (name == "TXT_REGION_PRISON_BLOCK_B" || name == "TXT_REGION_PRISON_BLOCK_C")
                text = L"";
            if (text != L"" && MiniMap.IsThingAsPosBeingDrawn(RegionExits[i].first)) {
                float scale = ScreenPosFadeInOut.GetFadeInOutAlpha() * 0.5f;
                scale += 0.5f;
                unsigned char text_alpha = (unsigned char)(ScreenPosFadeInOut.GetFadeInOutAlpha() * 255.0f);
                font.GetStringWidth(text);
                C2DVector text_pos = MiniMap.GetDrawPosition(RegionExits[i].first, map_box);
                PlayerGui->Draw2DText(C2DVector(GFAXToVX(text_pos.X), GFAYToVY(text_pos.Y)), text, font,
                    CRGBColour(0xff, 0xff, 0xff, ScaleAlpha(text_alpha)), scale, ENGINE_2D_LAYER_0, true);
            }
        }
    }
}

}

