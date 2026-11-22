// 00648820 NPlayerGui::CDrawMiniMap::Draw
// Flags: (none)
#define FABLE_RETAIL_STLP_VECTOR_CHECKED_SUBSCRIPT_PASS26
#define FABLE_RETAIL_CBASICSTRING_CHAR_COMPARE_MINIMAP_PASS26
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKDISPLAYENGINE
// Inline-budget: 0064887C calls bitset::test with the component bitset as its receiver.
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
// Leaf: ?Draw@CDrawMiniMap@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@_N@Z
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
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
#define FABLE_RETAIL_CDISPLAYENGINE_GETMAINWINDOW_PASS26
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISACTIVE
#define FABLE_RETAIL_CFADEINFADEOUTBASE_DECLS
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISCLOSING
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISOPENING_PASS26
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_NPLAYERGUI_CDRAWMINIMAP_TYPED_PASS26
#define FABLE_RETAIL_NPLAYERGUI_ANCHORS
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREENSCALED_PASS26
#define FABLE_RETAIL_CMINIMAPDISPLAY
#define FABLE_RETAIL_CMINIMAPDISPLAY_DRAWAPI_PASS26
#define FABLE_RETAIL_CTCREGIONDISPLAY
#define FABLE_RETAIL_CTCREGIONDISPLAY_ISDRAWING_PASS26
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
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#include "stlport/fable_containers.h"
// STLport test<274> at 00410DE0 retains its speed-mode exception path.
template<> inline bool std::bitset<274>::test(size_t n) const
{
    if (n >= 274)
        std::__stl_throw_out_of_range("bitset");
    return _Unchecked_test(n);
}
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif

#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DExtentsI.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CIntelligentPointer.h"
#ifdef _MSC_VER
#pragma optimize("s", off)
#endif
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#include "engine/CVectorMap.h"
#include "engine/ETCInterfaceType.h"
class CTCBase;
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::const_iterator CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType& key) const
{
 const_iterator first=this->begin(); long count=this->end()-first;
 while (count>0) { long half=count/2; const_iterator middle=first+half; if(middle->first<key) { first=middle+1;count-=half+1; } else count=half; }
 return first;
}
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif

#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#include "engine/CThing.h"
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/ThingAlive.h"
#include "engine/CTCRegionDisplay.h"
#ifdef _MSC_VER
#pragma optimize("s", off)
#endif
#include "engine/CEnginePrimitive2DSprite.h"
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
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

void CDrawMiniMap::Draw(const CEnginePrimitivePrepareInfo& info, bool drawing_hud)
{
    if (Active && !drawing_hud && Component.PeekDisplayEngine().IsLetterBoxMode()) {
        const CThingPlayerCreature* phero = GetPlayer().PeekPControlledCreature();
        if (IS_THING_ALIVE(phero)) {
            const CTCRegionDisplay* ptc_region_display;
            phero->PeekTC(&ptc_region_display);
            if (!ptc_region_display->IsDrawing())
                return;
        }
    }
    if (Active || RegionChange || FadeInOut.IsActive()) {
        SetAsDrawing(true);
        if (!World.IsPaused() && !Component.PeekDisplayEngine().IsScreenFadeOutClosing())
            ScreenPosFadeInOut.Update(info.Interpolate.Current.GTPredictedTimeSinceLastRenderFrame);

        C2DBoxF main_window = Component.PeekDisplayEngine().GetMainWindow();
        float map_size = GPDisplayManager->GetRenderTargetDimensions().X * 0.4f;
        C2DVector tl_pos((main_window.GetWidth() - map_size) * 0.5f, (main_window.GetHeight() - map_size) * 0.5f);
        C2DBoxF main_box(tl_pos, C2DExtentsF(map_size, map_size));
        C2DBoxF map_box = main_box;
        if (!RegionChange) {
            const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
            ScreenBox = C2DBoxF(GFAnchorToTopRight(gui_def.MiniMapScreenOrigin), C2DExtentsF(gui_def.MiniMapScreenSize.X, gui_def.MiniMapScreenSize.Y));
            map_box = ScreenBox;
            if (ScreenPosFadeInOut.IsOpen() || ScreenPosFadeInOut.IsOpening() || ScreenPosFadeInOut.IsClosing()) {
                map_box.TLX += ScreenPosFadeInOut.GetFadeInOutAlpha() * (main_box.TLX - ScreenBox.TLX);
                map_box.TLY += ScreenPosFadeInOut.GetFadeInOutAlpha() * (main_box.TLY - ScreenBox.TLY);
                map_box.BRX += ScreenPosFadeInOut.GetFadeInOutAlpha() * (main_box.BRX - ScreenBox.BRX);
                map_box.BRY += ScreenPosFadeInOut.GetFadeInOutAlpha() * (main_box.BRY - ScreenBox.BRY);
            }
        }

        float float_alpha = FadeInOut.GetFadeInOutAlpha() * 255.0f;
        if (float_alpha > 255.0f)
            float_alpha = 255.0f;
        unsigned char alpha = (unsigned char)float_alpha;
        if (alpha > Alpha)
            alpha = Alpha;
        unsigned char scaled_alpha = ScaleAlpha(alpha);
        if (RegionChange)
            scaled_alpha = alpha;
        MiniMap.DrawPreparePrimitivesForEngine(info, map_box, scaled_alpha, RegionChange);

        if (RegionChange) {
            const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
            C2DBoxF border_box;
            border_box.SetTLPos(gui_def.RegionChangeTLOffset);
            border_box.SetWidth((float)(GPDisplayManager->GetRenderTargetDimensions().X * Bank.GetGraphicMaxWidth(gui_def.RegionChangeTLGraphic)) * (1.0f / 640.0f));
            border_box.SetHeight((float)(Bank.GetGraphicMaxHeight(gui_def.RegionChangeTLGraphic) * GPDisplayManager->GetRenderTargetDimensions().Y) * (1.0f / 480.0f));
            CEnginePrimitive2DSprite border_tl_sprite(0, border_box, ENGINE_2D_LAYER_2, gui_def.RegionChangeTLGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2,
                C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[0], border_tl_sprite, 0xc0);

            border_box.SetTLPos(C2DVector(border_box.TLX + border_box.GetWidth(), border_box.TLY));
            border_box.SetWidth((float)(GPDisplayManager->GetRenderTargetDimensions().X * Bank.GetGraphicMaxWidth(gui_def.RegionChangeTMGraphic)) * (1.0f / 640.0f));
            border_box.SetHeight((float)(Bank.GetGraphicMaxHeight(gui_def.RegionChangeTMGraphic) * GPDisplayManager->GetRenderTargetDimensions().Y) * (1.0f / 480.0f));
            CEnginePrimitive2DSprite border_tm_sprite(0, border_box, ENGINE_2D_LAYER_2, gui_def.RegionChangeTMGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2,
                C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[1], border_tm_sprite, 0xc0);

            border_box.SetTLPos(C2DVector(border_box.TLX + border_box.GetWidth(), border_box.TLY));
            border_box.SetWidth((float)(GPDisplayManager->GetRenderTargetDimensions().X * Bank.GetGraphicMaxWidth(gui_def.RegionChangeTRGraphic)) * (1.0f / 640.0f));
            border_box.SetHeight((float)(Bank.GetGraphicMaxHeight(gui_def.RegionChangeTRGraphic) * GPDisplayManager->GetRenderTargetDimensions().Y) * (1.0f / 480.0f));
            CEnginePrimitive2DSprite border_tr_sprite(0, border_box, ENGINE_2D_LAYER_2, gui_def.RegionChangeTRGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2,
                C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[2], border_tr_sprite, 0xc0);

            border_box.SetTLPos(C2DVector(gui_def.RegionChangeTLOffset.X, border_box.BRY));
            border_box.SetWidth((float)(GPDisplayManager->GetRenderTargetDimensions().X * Bank.GetGraphicMaxWidth(gui_def.RegionChangeBLGraphic)) * (1.0f / 640.0f));
            border_box.SetHeight((float)(Bank.GetGraphicMaxHeight(gui_def.RegionChangeBLGraphic) * GPDisplayManager->GetRenderTargetDimensions().Y) * (1.0f / 480.0f));
            CEnginePrimitive2DSprite border_bl_sprite(0, border_box, ENGINE_2D_LAYER_2, gui_def.RegionChangeBLGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2,
                C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[3], border_bl_sprite, 0xc0);

            border_box.SetTLPos(C2DVector(border_box.TLX + border_box.GetWidth(), border_box.TLY));
            border_box.SetWidth((float)(GPDisplayManager->GetRenderTargetDimensions().X * Bank.GetGraphicMaxWidth(gui_def.RegionChangeBMGraphic)) * (1.0f / 640.0f));
            border_box.SetHeight((float)(Bank.GetGraphicMaxHeight(gui_def.RegionChangeBMGraphic) * GPDisplayManager->GetRenderTargetDimensions().Y) * (1.0f / 480.0f));
            CEnginePrimitive2DSprite border_bm_sprite(0, border_box, ENGINE_2D_LAYER_2, gui_def.RegionChangeBMGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2,
                C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[4], border_bm_sprite, 0xc0);

            border_box.SetTLPos(C2DVector(border_box.TLX + border_box.GetWidth(), border_box.TLY));
            border_box.SetWidth((float)(GPDisplayManager->GetRenderTargetDimensions().X * Bank.GetGraphicMaxWidth(gui_def.RegionChangeBRGraphic)) * (1.0f / 640.0f));
            border_box.SetHeight((float)(Bank.GetGraphicMaxHeight(gui_def.RegionChangeBRGraphic) * GPDisplayManager->GetRenderTargetDimensions().Y) * (1.0f / 480.0f));
            CEnginePrimitive2DSprite border_br_sprite(0, border_box, ENGINE_2D_LAYER_2, gui_def.RegionChangeBRGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2,
                C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[5], border_br_sprite, 0xc0);

            C2DBoxF left_pos(GFToScreen(gui_def.RegionChangeBorderLeftOffset), C2DExtentsF((float)Bank.GetGraphicMaxWidth(gui_def.RegionChangeBorderLeftGraphic), (float)Bank.GetGraphicMaxWidth(gui_def.RegionChangeBorderRightGraphic)));
            left_pos = GFToScreenScaled(left_pos);
            float graphic_height = (float)Bank.GetGraphicMaxHeight(gui_def.RegionChangeBorderLeftGraphic) * (GPDisplayManager->GetRenderTargetDimensions().Y * (1.0f / 480.0f));
            left_pos.TLY = (GPDisplayManager->GetRenderTargetDimensions().Y - graphic_height) * 0.5f;
            left_pos.BRY = left_pos.TLY + graphic_height;
            CEnginePrimitive2DSprite border_left_sprite(0, left_pos, ENGINE_2D_LAYER_1, gui_def.RegionChangeBorderLeftGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2,
                C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[6], border_left_sprite, 0xc0);

            C2DBoxF right_pos(GFToScreen(gui_def.RegionChangeBorderRightOffset), C2DExtentsF((float)Bank.GetGraphicMaxWidth(gui_def.RegionChangeBorderRightGraphic), (float)Bank.GetGraphicMaxHeight(gui_def.RegionChangeBorderRightGraphic)));
            right_pos = GFToScreenScaled(right_pos);
            right_pos.TLY = left_pos.TLY;
            right_pos.BRY = left_pos.BRY;
            CEnginePrimitive2DSprite border_right_sprite(0, right_pos, ENGINE_2D_LAYER_1, gui_def.RegionChangeBorderRightGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2,
                C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[7], border_right_sprite, 0xc0);
        }

        if (RegionChange) return;
        if (ScreenPosFadeInOut.IsActive()) {
            const CFontBank& font = Component.PeekCutSceneMainFont();
            // FableWin 020DF0A7 and Ego_d line 1078: VC6 loop-scope guard.
            if (false) {} else for (long i = 0; i < RegionExits.size(); ++i) {
                const CCharString& text_tag = RegionExits[i].second;
                CWideString text = Component.GetTextBank().GetTextBySymbol(text_tag);
                if (text == L"")
                    text = text_tag.ToWideString();
                if (RegionExits[i].second == "TXT_REGION_PRISON_BLOCK_B" || RegionExits[i].second == "TXT_REGION_PRISON_BLOCK_C")
                    text = L"";
                if (text != L"" && MiniMap.IsThingAsPosBeingDrawn(RegionExits[i].first)) {
                    float text_scale = ScreenPosFadeInOut.GetFadeInOutAlpha() * 0.5f;
                    text_scale += 0.5f;
                    unsigned char text_alpha = (unsigned char)(ScreenPosFadeInOut.GetFadeInOutAlpha() * 255.0f);
                    float string_width = font.GetStringWidth(text) * text_scale;
                    C2DVector text_pos = MiniMap.GetDrawPosition(RegionExits[i].first, map_box);
                    PlayerGui.Draw2DText(C2DVector(GFAXToVX(text_pos.X), GFAYToVY(text_pos.Y)), text, font,
                        CRGBColour(0xff, 0xff, 0xff, ScaleAlpha(text_alpha)), text_scale, ENGINE_2D_LAYER_0, true);
                }
            }
        }
    }
}

}
