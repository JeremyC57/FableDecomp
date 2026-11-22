// 00648820 NPlayerGui::CDrawMiniMap::Draw behavior fixture
#ifndef _MSC_VER
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define __fastcall
#include <stdint.h>
typedef uint64_t SpriteId;
#else
typedef unsigned __int64 SpriteId;
#endif
#define FABLE_RETAIL_STLP_VECTOR_CHECKED_SUBSCRIPT_PASS26
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_SETTERS
#define FABLE_RETAIL_C2DBOXF_EXTENTS_DECL
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRGBCOLOUR
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DExtentsI.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#ifdef _MSC_VER
#include "stlport/fable_containers.h"
#else
#include <utility>
#include <vector>
#endif
#include <cstdio>
#include <cstring>
#include <wchar.h>
#include <cmath>

static unsigned failures;
#define CHECK(x) do { if (!(x)) { if (failures < 15) std::printf("line %d: %s\n", __LINE__, #x); ++failures; } } while (0)
static bool Near(float a, float b) { return std::fabs(a-b) < 0.001; }
static bool Same(const C2DBoxF& a,const C2DBoxF& b)
{
    return Near(a.TLX,b.TLX) && Near(a.TLY,b.TLY) && Near(a.BRX,b.BRX) && Near(a.BRY,b.BRY);
}

struct DisplayManager {
    C2DExtentsI GetRenderTargetDimensions() const { return C2DExtentsI(1280, 960); }
};
static DisplayManager Display;
static DisplayManager* GPDisplayManager = &Display;
struct DisplayEngine {
    bool letterbox, closing;
    bool IsLetterBoxMode() const { return letterbox; }
    bool IsScreenFadeOutClosing() const { return closing; }
    C2DBoxF GetMainWindow() const { return C2DBoxF(0,0,1280,960); }
};
struct RegionDisplay {
    bool drawing;
    bool IsDrawing() const { return drawing; }
};
const long TCI_REGION_DISPLAY = 0xb5;
struct PlayerCreature {
    bool alive;
    RegionDisplay region;
    mutable unsigned regionQueries;
    PlayerCreature() : alive(false), regionQueries(0) {}
    bool PeekTC(const RegionDisplay** component) const
    {
        *component = &region;
        ++regionQueries;
        return true;
    }
};
static bool IS_THING_ALIVE(const PlayerCreature* hero) { return hero && hero->alive; }
struct Player {
    const PlayerCreature* hero;
    const PlayerCreature* PeekPControlledCreature() const { return hero; }
};
struct Players {
    Player main;
    const Player& GetMainPlayer() const { return main; }
};
struct RenderWorld {
    bool paused;
    bool IsPaused() const { return paused; }
};
struct Fade {
    unsigned state;
    float alpha, lastTime;
    unsigned updates;
    Fade() : state(0), alpha(0), lastTime(-1), updates(0) {}
    bool IsActive() const { return state != 0; }
    bool IsOpen() const { return state == 1; }
    bool IsOpening() const { return state == 2; }
    bool IsClosing() const { return state == 3; }
    void Update(float time) { ++updates; lastTime = time; }
    float GetFadeInOutAlpha() const { return alpha; }
};

struct Wide {
    const wchar_t* chars;
    Wide(const wchar_t* value) : chars(value) {}
    bool operator==(const wchar_t* value) const { return wcscmp(chars, value)==0; }
    bool operator!=(const wchar_t* value) const { return !(*this == value); }
};
struct Narrow {
    const char* chars;
    const wchar_t* fallback;
    Narrow(const char* name, const wchar_t* alt) : chars(name), fallback(alt) {}
    Wide ToWideString() const { return Wide(fallback); }
    bool operator==(const char* value) const { return std::strcmp(chars,value)==0; }
};
struct TextBank {
    Wide GetTextBySymbol(const Narrow& symbol) const
    {
        return symbol=="localized" ? Wide(L"Translated") : Wide(L"");
    }
};
struct Font {
    mutable unsigned widths;
    Font() : widths(0) {}
    float GetStringWidth(const Wide& value) const { ++widths; return (float)wcslen(value.chars); }
};
struct Component {
    DisplayEngine display;
    Font font;
    TextBank bank;
    const DisplayEngine& PeekDisplayEngine() const { return display; }
    const Font& PeekCutSceneMainFont() const { return font; }
    const TextBank& GetTextBank() const { return bank; }
};
struct GuiDef {
    C2DVector MiniMapScreenOrigin, MiniMapScreenSize;
    C2DVector RegionChangeTLOffset, RegionChangeBorderLeftOffset, RegionChangeBorderRightOffset;
    unsigned long RegionChangeTLGraphic, RegionChangeTMGraphic, RegionChangeTRGraphic;
    unsigned long RegionChangeBLGraphic, RegionChangeBMGraphic, RegionChangeBRGraphic;
    unsigned long RegionChangeBorderLeftGraphic, RegionChangeBorderRightGraphic;
};
static GuiDef Definition;
enum EEngine2DLayer { ENGINE_2D_LAYER_0, ENGINE_2D_LAYER_1, ENGINE_2D_LAYER_2 };
enum { ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0 };

struct Gui {
    unsigned labels;
    C2DVector labelsAt[2];
    const wchar_t* labelsText[2];
    unsigned char labelsAlpha[2];
    float lastScale;
    Gui() : labels(0), lastScale(0) {}
    static const GuiDef& PeekGuiDef() { return Definition; }
    void Draw2DText(const C2DVector& at, const Wide& text, const Font&, const CRGBColour& colour,
                    float scale, EEngine2DLayer layer, bool centered)
    {
        CHECK(centered && layer==ENGINE_2D_LAYER_0 && colour.R==255 && colour.G==255 && colour.B==255);
        if (labels<2) {
            labelsAt[labels]=at;
            labelsText[labels]=text.chars;
            labelsAlpha[labels]=colour.A;
        }
        ++labels;
        lastScale=scale;
    }
};
struct GraphicsBank {
    long GetGraphicMaxWidth(unsigned long id) const { return 20 + id * 2; }
    long GetGraphicMaxHeight(unsigned long id) const { return 10 + id; }
};

struct Sprite {
    C2DBoxF box;
    EEngine2DLayer layer;
    unsigned long graphic;
    unsigned char alpha;
    Sprite(SpriteId id,const C2DBoxF& area,EEngine2DLayer requestedLayer,unsigned long graphicId,
           unsigned long frame,int blend,int interpolation,float start,float end,unsigned char opacity,
           unsigned char r,unsigned char g,unsigned char b,unsigned long flags,const C2DVector& uv0,const C2DVector& uv1)
        : box(area), layer(requestedLayer), graphic(graphicId), alpha(opacity)
    {
        CHECK(!id && !frame && blend==ENGINE_PRIMITIVE_BLEND_ALPHA && interpolation==ENGINE_INTERPOLATION_SET_0);
        CHECK(start==0.0f && end==0.0f && r==255 && g==255 && b==255 && flags==2);
        CHECK(uv0.X==0.0f && uv0.Y==0.0f && uv1.X==0.0f && uv1.Y==0.0f);
    }
};
struct EngineCapture {
    unsigned count;
    C2DBoxF boxes[8];
    unsigned char alpha[8];
    EngineCapture() : count(0) {}
    void SetPersistentPrimitive(unsigned long handle,const Sprite& sprite,unsigned long flags)
    {
        CHECK(count<8 && handle==count && sprite.graphic==count && flags==0xc0);
        CHECK(sprite.layer==(count<6 ? ENGINE_2D_LAYER_2 : ENGINE_2D_LAYER_1));
        if (count<8) {
            boxes[count]=sprite.box;
            alpha[count]=sprite.alpha;
        }
        ++count;
    }
};
struct Prepare {
    EngineCapture& Engine;
    struct { struct { float GTPredictedTimeSinceLastRenderFrame; } Current; } Interpolate;
    explicit Prepare(EngineCapture& engine) : Engine(engine) { Interpolate.Current.GTPredictedTimeSinceLastRenderFrame=0.25f; }
};
struct Mini {
    unsigned draws, visibleQueries, positionQueries;
    C2DBoxF map;
    unsigned char alpha;
    bool changing;
    Mini() : draws(0), visibleQueries(0), positionQueries(0), alpha(0), changing(false) {}
    void DrawPreparePrimitivesForEngine(const Prepare&,const C2DBoxF& area,unsigned char opacity,bool changingRegion)
    {
        ++draws;
        map=area;
        alpha=opacity;
        changing=changingRegion;
    }
    bool IsThingAsPosBeingDrawn(const C3DVector& position) { ++visibleQueries; return position.Z>0; }
    C2DVector GetDrawPosition(const C3DVector& position,const C2DBoxF& area)
    {
        CHECK(Same(map,area));
        ++positionQueries;
        return C2DVector(position.X+map.TLX, position.Y+map.TLY);
    }
};
static C2DVector GFToScreen(const C2DVector& offset) { return offset; }
static C2DVector GFAnchorToTopRight(const C2DVector& position) { return C2DVector(1280-position.X,position.Y); }
static float GFAXToVX(float value) { return value*0.5f; }
static float GFAYToVY(float value) { return value*0.5f; }

struct FixtureDrawer {
    bool Active, RegionChange;
    Players& PlayerManager;
    ::RenderWorld& World;
    const ::Component& Component;
    Gui& PlayerGui;
    const GraphicsBank& Bank;
    unsigned char Alpha;
    unsigned PrimitiveHandles[8];
    Fade FadeInOut, ScreenPosFadeInOut;
    C2DBoxF ScreenBox;
    Mini MiniMap;
    std::vector<std::pair<C3DVector,Narrow> > RegionExits;
    unsigned drawing, scales;
    FixtureDrawer(Players& players, RenderWorld& world, const ::Component& component, Gui& gui, const GraphicsBank& bank) : PlayerManager(players), World(world), Component(component), PlayerGui(gui), Bank(bank), ScreenBox(-4,-3,-2,-1), drawing(0), scales(0)
    {
        for (unsigned i=0;i!=8;++i)
            PrimitiveHandles[i]=i;
    }
    const Player& GetPlayer() { return PlayerManager.GetMainPlayer(); }
    void SetAsDrawing(bool value) { CHECK(value); ++drawing; }
    unsigned char ScaleAlpha(unsigned char alpha) { ++scales; return alpha/2; }
    void Draw(const Prepare&,bool);
};

#define CDisplayManager DisplayManager
#define CDrawMiniMap FixtureDrawer
#define CEnginePrimitivePrepareInfo Prepare
#define CThingPlayerCreature PlayerCreature
#define CTCRegionDisplay RegionDisplay
#define CPlayerGuiDef GuiDef
#define CPlayerGui Gui
#define CEnginePrimitive2DSprite Sprite
#define CFontBank Font
#define CCharString Narrow
#define CWideString Wide

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


int main()
{
    Definition.MiniMapScreenOrigin=C2DVector(240,25);
    Definition.MiniMapScreenSize=C2DVector(160,180);
    Definition.RegionChangeTLOffset=C2DVector(11,17);
    Definition.RegionChangeBorderLeftOffset=C2DVector(20,23);
    Definition.RegionChangeBorderRightOffset=C2DVector(600,31);
    Definition.RegionChangeTLGraphic=0; Definition.RegionChangeTMGraphic=1; Definition.RegionChangeTRGraphic=2;
    Definition.RegionChangeBLGraphic=3; Definition.RegionChangeBMGraphic=4; Definition.RegionChangeBRGraphic=5;
    Definition.RegionChangeBorderLeftGraphic=6; Definition.RegionChangeBorderRightGraphic=7;
    const unsigned scenarios=512*5*3;
    for (unsigned n=0;n!=scenarios;++n) {
        const unsigned bits=n%512;
        const unsigned screenState=(n/512)%5;
        const unsigned opacity=n/(512*5);
        const bool active=(bits&1)!=0, hud=(bits&2)!=0, letterbox=(bits&4)!=0;
        const bool region=(bits&8)!=0, mainFade=(bits&16)!=0, paused=(bits&32)!=0, closing=(bits&64)!=0;
        const bool heroAlive=(bits&128)!=0, regionVisible=(bits&256)!=0;
        Players players;
        PlayerCreature hero;
        players.main.hero=heroAlive || regionVisible ? &hero : 0;
        hero.alive=heroAlive;
        hero.region.drawing=regionVisible;
        RenderWorld world; world.paused=paused;
        Component component; component.display.letterbox=letterbox; component.display.closing=closing;
        GraphicsBank bank;
        Gui gui;
        FixtureDrawer drawer(players,world,component,gui,bank);
        EngineCapture engine;
        Prepare info(engine);
        drawer.Active=active; drawer.RegionChange=region;
        drawer.Alpha=opacity==2 ? 200 : 255;
        drawer.FadeInOut.state=mainFade ? 1 : 0;
        drawer.FadeInOut.alpha=opacity==0 ? 0.5f : 1.5f;
        drawer.ScreenPosFadeInOut.state=screenState;
        drawer.ScreenPosFadeInOut.alpha=0.25f;
        drawer.RegionExits.push_back(std::make_pair(C3DVector(1,2,1),Narrow("localized",L"fallback")));
        drawer.RegionExits.push_back(std::make_pair(C3DVector(2,3,1),Narrow("other",L"Fallback")));
        drawer.RegionExits.push_back(std::make_pair(C3DVector(2,3,1),Narrow("TXT_REGION_PRISON_BLOCK_B",L"hidden")));
        drawer.RegionExits.push_back(std::make_pair(C3DVector(2,3,1),Narrow("TXT_REGION_PRISON_BLOCK_C",L"hidden")));
        drawer.RegionExits.push_back(std::make_pair(C3DVector(2,3,0),Narrow("unseen",L"unseen")));
        drawer.RegionExits.push_back(std::make_pair(C3DVector(2,3,1),Narrow("empty",L"")));

        drawer.Draw(info,hud);

        const bool checksRegion=active && !hud && letterbox && heroAlive;
        const bool draws=!(checksRegion && !regionVisible) && (active || region || mainFade);
        CHECK(hero.regionQueries==(checksRegion ? 1U : 0U));
        CHECK(drawer.drawing==(draws ? 1U : 0U) && drawer.MiniMap.draws==drawer.drawing);
        const bool updates=draws && !paused && !closing;
        CHECK(drawer.ScreenPosFadeInOut.updates==(updates ? 1U : 0U));
        CHECK(drawer.ScreenPosFadeInOut.lastTime==(updates ? 0.25f : -1.0f));
        const bool labels=draws && !region && screenState!=0;
        CHECK(gui.labels==(labels ? 2U : 0U) && component.font.widths==gui.labels);
        CHECK(drawer.MiniMap.visibleQueries==(labels ? 3U : 0U) && drawer.MiniMap.positionQueries==gui.labels);
        CHECK(drawer.scales==(draws ? 1U+gui.labels : 0U));
        CHECK(engine.count==(draws && region ? 8U : 0U));
        if (!draws)
            continue;

        const unsigned char alpha=opacity==0 ? 127 : (opacity==1 ? 255 : 200);
        CHECK(drawer.MiniMap.alpha==(region ? alpha : alpha/2) && drawer.MiniMap.changing==region);
        const C2DBoxF spot(1040,25,1200,205);
        const C2DBoxF center(384,224,896,736);
        const bool animated=screenState>=1 && screenState<=3;
        const C2DBoxF ease(876,74.75f,1124,337.75f);
        CHECK(Same(drawer.MiniMap.map,region ? center : (animated ? ease : spot)));
        CHECK(Same(drawer.ScreenBox,region ? C2DBoxF(-4,-3,-2,-1) : spot));
        if (labels) {
            CHECK(wcscmp(gui.labelsText[0],L"Translated")==0 && wcscmp(gui.labelsText[1],L"Fallback")==0);
            CHECK(gui.labelsAlpha[0]==31 && gui.labelsAlpha[1]==31 && gui.lastScale==0.625f);
            CHECK(Near(gui.labelsAt[0].X,(drawer.MiniMap.map.TLX+1)*0.5f));
            CHECK(Near(gui.labelsAt[1].Y,(drawer.MiniMap.map.TLY+3)*0.5f));
        }
        if (region) {
            const float left[6]={11,51,95,11,63,119};
            for (unsigned i=0;i!=6;++i) {
                const float top=i<3 ? 17.0f : 41.0f;
                CHECK(Same(engine.boxes[i],C2DBoxF(left[i],top,left[i]+2*bank.GetGraphicMaxWidth(i),
                                                 top+2*bank.GetGraphicMaxHeight(i))));
            }
            CHECK(Same(engine.boxes[6],C2DBoxF(40,464,104,496)));
            CHECK(Same(engine.boxes[7],C2DBoxF(1200,464,1268,496)));
            for (unsigned i=0;i!=8;++i)
                CHECK(engine.alpha[i]==alpha);
        }
    }
    std::puts(failures ? "00648820_TEST FAIL" : "00648820_TEST PASS");
    return failures ? 1 : 0;
}