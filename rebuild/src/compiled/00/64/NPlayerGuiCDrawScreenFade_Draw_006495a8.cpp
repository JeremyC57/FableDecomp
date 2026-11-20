// NPlayerGui::CDrawScreenFade::Draw(const CEnginePrimitivePrepareInfo& info) -- the full-screen fade box: black over the virtual screen while the save/load-failed flag is set; else, unless the hero's quest-completion UI overrides the fade, while the display engine's screen fade is active, its colour (alpha from the engine) over the virtual screen grown by 8 units -- layer 3 when the full fade-out was drawn and the region-load minimap shows during a region load, layer 8 while the hero's receive-items UI is displaying or the subtitles show text with flag 8, layer 3 in a skippable-while-paused cutscene while paused, else layer M1 (remembering a fully opaque fade-out); with no fade, clear the remembered fade-out
// Inline-budget: __declspec(noinline) through the header opt-in FABLE_RETAIL_IS_THING_ALIVE_NOINLINE (engine/ThingAlive.h) -- retail calls the IS_THING_ALIVE<const CThingPlayerCreature*> COMDAT 0x0043DBF0 out of line twice (0x00649666, 0x006497D3); without it DIFFER(977v938). Also the CThing.h header contract FABLE_RETAIL_CTHING_GETTC_EXPANDED (PeekTC<T> __forceinline, owner decision 2026-11-01: no out-of-line PeekTC in Ego_r.pdb or Fable.exe; without it DIFFER(901v938)). Both are the contracts the TU-mate CDrawFishingGame::Draw 0x00649E8B uses. No flag, no pragma.
// Leaf: ?Draw@CDrawScreenFade@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006495A8, 938 bytes, thiscall, `ret 4`; size TU (#pragma optimize("s"): biased ebp frame `lea ebp,[esp-74h]`).
// Identity: Ego_d.pdb ?Draw@CDrawScreenFade@NPlayerGui@@ (debug length 1760; locals this, info, phero, display_engine; { clip_box,
//   background_sprite }; { ptc_quest_ui }; { clip_box, colour, { background_sprite }, { background_sprite, { ptc_receive_items,
//   { background_sprite } }, { background_sprite }, { background_sprite } } }); caller CPlayerGui::DrawScreenFade 0x00437670.
// Shape (Ego_d.pdb C13 lines 1404..1498, egod_lines.tsv): `if (g_bSaveLoadFailed)` first (15 B at /Od, its jcc opens the first block),
//   phero assigned after that block (21 B: GetPlayer().PeekPControlledCreature()), the pointer components initialised to 0 (`and [x],0`:
//   4 B / 7 B), display_engine taken after the quest-UI test (17 B), the colour copied (17 B: a copy-constructor call) then its alpha set
//   (11 B), four separate sprite blocks -- the receive-items one nested in IS_THING_ALIVE / PeekTC / IsDisplayingItems, the subtitle test
//   split over two lines (32 + 34 B: IsDisplayingText, IsFlagSet(8)), the cutscene test one line (121 B = IsPaused + three subtitle calls).
//   VC7.1 cross-jumps the three tail sprite blocks into the first (0x00649887 `jmp`, 0x006498B4 `jne`).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::PeekPControlledCreature 0x00487DD0, IS_THING_ALIVE<const CThingPlayerCreature*>
//   0x0043DBF0, std::bitset::test 0x00410DE0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0,
//   CTCQuestCompletionUI::IsOverridingScreenFade 0x005E3E70, GFAXToVX 0x009E1DA0, CDisplayEngine::GetScreenFadeOutAlpha 0x004364C0 /
//   IsScreenFullyFadedOut 0x004A9210, CWorld::IsDisplayingMiniMapOnRegionLoad 0x0049E9F0 / IsPerformingRegionLoad 0x0048DB13,
//   CSubtitleRenderer::IsCutsceneSkippable 0x0048DE44, CEnginePrimitive2DSprite ctor 0x0041BEB0, CIEngine::SetPersistentPrimitive (slot 23).
// Data: g_bSaveLoadFailed 0x013B89C0 (descriptive name, as the landed rows 0x00416953 / 0x00498490 / 0x004A5A40 spell it), VIRTUAL_SCREEN_WIDTH
//   0x013961E8 / VIRTUAL_SCREEN_HEIGHT 0x013961F0 (Ego_r.pdb publics ?VIRTUAL_SCREEN_WIDTH@@3NA / ..HEIGHT..), 8.0f 0x0125916C, -8.0f 0x0125A298.
// Overlays (add-only opt-ins): CDisplayEngine.h FABLE_RETAIL_CDISPLAYENGINE_SCREENFADE_DRAW, CFadeInFadeOutBase.h ..._GETCOLOUR,
//   CSubtitleRenderer.h ..._SKIPPABLEWHILEPAUSED, CWorld.h FABLE_RETAIL_CWORLD_SCREENFADE_CALLEES, CTCQuestCompletionUI.h
//   ..._ISOVERRIDINGSCREENFADE, CTCHeroReceiveItems.h ..._ISDISPLAYINGITEMS.
// CDrawScreenFade has no shared header: the class is declared here on the shared NPlayerGui::CDrawBase (Ego_r.pdb: DrawnFullFadeOut, retail
//   +0x28; the landed ctor 0x0064E8DC clears it), as the TU-mate rows 0x00649E8B (CDrawFishingGame) declare theirs.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKDISPLAYENGINE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_GETPLAYER
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_BOX_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDISPLAYENGINE_SCREENFADEOUT
#define FABLE_RETAIL_CDISPLAYENGINE_REGIONLOADFADE
#define FABLE_RETAIL_CDISPLAYENGINE_SCREENFADE_DRAW
#define FABLE_RETAIL_CFADEINFADEOUTBASE_FADE
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISACTIVE
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISCLOSING
#define FABLE_RETAIL_CFADEINFADEOUTBASE_GETCOLOUR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_UPDATEINPUT
#define FABLE_RETAIL_CWORLD_SCREENFADE_CALLEES
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_BUTTONGRAPHICS
#define FABLE_RETAIL_CSUBTITLERENDERER_ISFLAGSET
#define FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE
#define FABLE_RETAIL_CSUBTITLERENDERER_SKIPPABLEWHILEPAUSED
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_ISOVERRIDINGSCREENFADE
#define FABLE_RETAIL_CTCHERORECEIVEITEMS_PICKEDUPITEMS
#define FABLE_RETAIL_CTCHERORECEIVEITEMS_DISPLAYINGITEMS
#define FABLE_RETAIL_CTCHERORECEIVEITEMS_ISDISPLAYINGITEMS
#include "engine/C2DVector.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/ThingAlive.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/CPlayerGui.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/CWorld.h"
#include "engine/CDisplayEngine.h"
#include "engine/CTCQuestCompletionUI.h"
#include "engine/CTCHeroReceiveItems.h"
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/NPlayerGui_Graphics.h"
extern bool g_bSaveLoadFailed;                                   // 0x013B89C0
extern double VIRTUAL_SCREEN_WIDTH;                              // 0x013961E8 ?VIRTUAL_SCREEN_WIDTH@@3NA
extern double VIRTUAL_SCREEN_HEIGHT;                             // 0x013961F0 ?VIRTUAL_SCREEN_HEIGHT@@3NA
float FABLE_FASTCALL GFAXToVX(float x);                          // 0x009E1DA0 ?GFAXToVX@@YIMM@Z
namespace NPlayerGui {
class CDrawScreenFade : public CDrawBase {
public:
    void Draw(const CEnginePrimitivePrepareInfo& info);
private:
    bool DrawnFullFadeOut;                                       // +0x28
};
void CDrawScreenFade::Draw(const CEnginePrimitivePrepareInfo& info)
{
    const CThingPlayerCreature* phero;
    if (g_bSaveLoadFailed) {
        SetAsDrawing(true);
        C2DBoxF clip_box(0.0f, 0.0f, (float)VIRTUAL_SCREEN_WIDTH, (float)VIRTUAL_SCREEN_HEIGHT);
        CEnginePrimitive2DSprite background_sprite(0, GFToScreen(clip_box), ENGINE_2D_LAYER_8, 0, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
            ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, 255, 0, 0, 0, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[0], background_sprite, 0xc0);
        return;
    }
    phero = GetPlayer().PeekPControlledCreature();
    if (IS_THING_ALIVE(phero)) {
        const CTCQuestCompletionUI* ptc_quest_ui = 0;
        if (phero->PeekTC(&ptc_quest_ui))
            if (ptc_quest_ui->IsOverridingScreenFade())
                return;
    }
    const CDisplayEngine& display_engine = Component->PeekDisplayEngine();
    if (display_engine.IsScreenFadeOut()) {
        SetAsDrawing(true);
        C2DBoxF clip_box(-8.0f, -8.0f, GFAXToVX(8.0f) + VIRTUAL_SCREEN_WIDTH, GFAXToVX(8.0f) + VIRTUAL_SCREEN_HEIGHT);
        CRGBColour colour = display_engine.GetScreenFadeOutColour();
        colour.A = display_engine.GetScreenFadeOutAlpha();
        if (DrawnFullFadeOut && display_engine.IsScreenFullyFadedOut() && World->IsDisplayingMiniMapOnRegionLoad() && World->IsPerformingRegionLoad()) {
            CEnginePrimitive2DSprite background_sprite(0, GFToScreen(clip_box), ENGINE_2D_LAYER_3, 0, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
                ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, colour.A, colour.R, colour.G, colour.B, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine->SetPersistentPrimitive(PrimitiveHandles[0], background_sprite, 0xc0);
        } else {
            if (IS_THING_ALIVE(phero)) {
                const CTCHeroReceiveItems* ptc_receive_items = 0;
                if (phero->PeekTC(&ptc_receive_items))
                    if (ptc_receive_items->IsDisplayingItems()) {
                        CEnginePrimitive2DSprite background_sprite(0, GFToScreen(clip_box), ENGINE_2D_LAYER_8, 0, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
                            ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, colour.A, colour.R, colour.G, colour.B, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                        info.Engine->SetPersistentPrimitive(PrimitiveHandles[0], background_sprite, 0xc0);
                        return;
                    }
            }
            if (PlayerGui->GetSubtitleRenderer().IsDisplayingText() &&
                PlayerGui->GetSubtitleRenderer().IsFlagSet((ESubtitleFlag)8)) {
                CEnginePrimitive2DSprite background_sprite(0, GFToScreen(clip_box), ENGINE_2D_LAYER_8, 0, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
                    ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, colour.A, colour.R, colour.G, colour.B, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                info.Engine->SetPersistentPrimitive(PrimitiveHandles[0], background_sprite, 0xc0);
                return;
            }
            if (World->IsPaused() && PlayerGui->GetSubtitleRenderer().IsInCutscene() && PlayerGui->GetSubtitleRenderer().IsCutsceneSkippable() && PlayerGui->GetSubtitleRenderer().IsCutsceneSkippableWhilePaused()) {
                CEnginePrimitive2DSprite background_sprite(0, GFToScreen(clip_box), ENGINE_2D_LAYER_3, 0, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
                    ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, colour.A, colour.R, colour.G, colour.B, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                info.Engine->SetPersistentPrimitive(PrimitiveHandles[0], background_sprite, 0xc0);
                return;
            }
            CEnginePrimitive2DSprite background_sprite(0, GFToScreen(clip_box), ENGINE_2D_LAYER_M1, 0, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
                ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, colour.A, colour.R, colour.G, colour.B, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine->SetPersistentPrimitive(PrimitiveHandles[0], background_sprite, 0xc0);
            if (colour.A == 255)
                DrawnFullFadeOut = true;
        }
    } else if (DrawnFullFadeOut)
        DrawnFullFadeOut = false;
}
}