#define FABLE_RETAIL_C2DVECTOR
#include "engine/C2DVector.h"
#include <cassert>
#include <cstdio>

enum EEngine2DLayer { HUD_LAYER=0x46 };
enum EEnginePrimitiveBlendMode { ENGINE_PRIMITIVE_BLEND_ALPHA };
enum EEngineInterpolationSet { ENGINE_INTERPOLATION_SET_0 };
enum { TCI_INVENTORY };

struct CEnginePrimitive2DSprite
{
    long graphic;
    C2DVector point;
    unsigned char opacity;

    CEnginePrimitive2DSprite(long id,const C2DVector& p,EEngine2DLayer layer,long g,long texture_frame,
        EEnginePrimitiveBlendMode blend,EEngineInterpolationSet interpolation,float scale,float rotation,
        unsigned char alpha,unsigned char red,unsigned char green,unsigned char blue,long style,
        const C2DVector& offsets,const C2DVector& alignment)
        : graphic(g), point(p), opacity(alpha)
    {
        assert(id==0 && layer==HUD_LAYER && texture_frame==0);
        assert(blend==ENGINE_PRIMITIVE_BLEND_ALPHA && interpolation==ENGINE_INTERPOLATION_SET_0);
        assert(scale==0 && rotation==0 && red==255 && green==255 && blue==255 && style==0);
        assert(offsets.X==0 && offsets.Y==0 && alignment.X==0 && alignment.Y==0);
    }
};

struct CEnginePrimitiveHandle
{
    long graphic;
    C2DVector pos;
    unsigned alpha;
    unsigned updates, clears;

    CEnginePrimitiveHandle() : graphic(-2), pos(0,0), alpha(0), updates(0), clears(0) { }
    void Clear() { graphic=0; ++clears; }
};

class CIEngine
{
public:
    void SetPersistentPrimitive(CEnginePrimitiveHandle& handle,const CEnginePrimitive2DSprite& sprite,long flags)
    {
        assert(flags==0xc0);
        handle.graphic=sprite.graphic;
        handle.pos=sprite.point;
        handle.alpha=sprite.opacity;
        ++handle.updates;
    }
};

class CEnginePrimitivePrepareInfo
{
public:
    CIEngine& Engine;
    explicit CEnginePrimitivePrepareInfo(CIEngine& engine) : Engine(engine) { }
};

class CGraphicDataBank
{
public:
    mutable unsigned width_reads;
    CGraphicDataBank() : width_reads(0) { }
    float GetGraphicMaxWidth(long graphic) const
    {
        assert(graphic>=21 && graphic<=23);
        ++width_reads;
        return 16.0f;
    }
};

class CPlayerGuiDef
{
public:
    struct { C2DVector Offset; } QuickAccessItemButtons[4];
    C2DVector DPadRingsPos, HotBarOffset, HotBarEndOffset;
    long HotBarSingleGraphic, HotBarMainGraphic, HotBarEndGraphics[3];
};

class CPlayerGui
{
public:
    CPlayerGuiDef gui_def;
    CGraphicDataBank graphic_bank;

    static CPlayerGui& Get() { static CPlayerGui gui; return gui; }
    static const CPlayerGuiDef& PeekGuiDef() { return Get().gui_def; }
    const CGraphicDataBank& PeekGraphicBank() const { return graphic_bank; }
};

class CTCInventory
{
public:
    long graphics[4];
    mutable unsigned reads;
    CTCInventory() : reads(0) { }
    long GetQuickAccessItemGraphicIndex(long index) const
    { assert(index>=0 && index<4); ++reads; return graphics[index]; }
};

class CThingPlayerCreature
{
public:
    bool alive;
    CTCInventory inventory;
    CThingPlayerCreature() : alive(true) { }
    bool PeekTC(int type,const CTCInventory** tc) const
    { assert(type==TCI_INVENTORY); *tc=&inventory; return true; }
};

bool IS_THING_ALIVE(const CThingPlayerCreature* hero) { return hero && hero->alive; }

class CGamePlayerInterface
{
public:
    long GetNumberOfQuickAccessItemButtons() const { return 4; }
};

class CPlayer
{
public:
    const CThingPlayerCreature* hero;
    CGamePlayerInterface interface;
    CPlayer() : hero(0) { }
    const CThingPlayerCreature* PeekPControlledCreature() const { return hero; }
    const CGamePlayerInterface* GetPlayerInterface() const { return &interface; }
};

long HUDRingAlpha=0x1889;

namespace NPlayerGui {

C2DVector GFAnchorToBottomLeft(const C2DVector& anchor) { return anchor; }
C2DVector GFToScreen(const C2DVector& point) { return C2DVector(point.X*2.0f,point.Y*4.0f); }

class CDrawQuickAccessIcons
{
public:
    unsigned bases;
    bool drawing;
    CPlayer player;
    CDrawQuickAccessIcons() : bases(0), drawing(false) { }
    void Draw(const CEnginePrimitivePrepareInfo&) { ++bases; }
    const CPlayer& GetPlayer() const { return player; }
    void SetAsDrawing(bool value) { drawing=value; }
    unsigned char ScaleAlpha(unsigned char alpha) const { return alpha>>1; }
};

class CDrawQuickAccessIconsPC : public CDrawQuickAccessIcons
{
public:
    CEnginePrimitiveHandle HotBarPrimitiveHandles[2];
    void Draw(const CEnginePrimitivePrepareInfo& info);
};

}

// The selected implementation is reproduced verbatim for this standalone fixture.
namespace NPlayerGui {

void CDrawQuickAccessIconsPC::Draw(const CEnginePrimitivePrepareInfo& info)
{
    CDrawQuickAccessIcons::Draw(info);
    const CThingPlayerCreature* phero = GetPlayer().PeekPControlledCreature();
    if (IS_THING_ALIVE(phero)) {
        const CTCInventory* ptc_inventory = 0;
        phero->PeekTC(TCI_INVENTORY, &ptc_inventory);
        long num_allocated_quick_access_buttons = 0;
        long num_quick_access_slots = GetPlayer().GetPlayerInterface()->GetNumberOfQuickAccessItemButtons();
        for (long i = 0; i < num_quick_access_slots; ++i) {
            if (ptc_inventory->GetQuickAccessItemGraphicIndex(i) > 0)
                num_allocated_quick_access_buttons = i + 1;
        }
        if (num_allocated_quick_access_buttons > 0) {
            SetAsDrawing(true);
            const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
            if (num_allocated_quick_access_buttons == 1) {
                const C2DVector& offset = gui_def.QuickAccessItemButtons[0].Offset;
                C2DVector hot_bar_pos = offset + GFAnchorToBottomLeft(gui_def.DPadRingsPos) + gui_def.HotBarOffset;
                CEnginePrimitive2DSprite hot_bar_sprite(0, GFToScreen(hot_bar_pos), (EEngine2DLayer)0x46, gui_def.HotBarSingleGraphic, 0,
                    ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha((unsigned char)HUDRingAlpha), 0xff, 0xff,
                    0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                info.Engine.SetPersistentPrimitive(HotBarPrimitiveHandles[0], hot_bar_sprite, 0xc0);
                HotBarPrimitiveHandles[1].Clear();
            } else {
                C2DVector hot_bar_pos = GFAnchorToBottomLeft(gui_def.DPadRingsPos) +
                    gui_def.QuickAccessItemButtons[num_allocated_quick_access_buttons - 1].Offset + gui_def.HotBarOffset;
                CEnginePrimitive2DSprite hot_bar_sprite(0, GFToScreen(hot_bar_pos), (EEngine2DLayer)0x46, gui_def.HotBarMainGraphic, 0,
                    ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha((unsigned char)HUDRingAlpha), 0xff, 0xff,
                    0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                info.Engine.SetPersistentPrimitive(HotBarPrimitiveHandles[0], hot_bar_sprite, 0xc0);
                long end_graphic = gui_def.HotBarEndGraphics[num_allocated_quick_access_buttons - 2];
                if (end_graphic > 0) {
                    const CGraphicDataBank& graphic_bank = CPlayerGui::Get().PeekGraphicBank();
                    hot_bar_pos.X = hot_bar_pos.X - graphic_bank.GetGraphicMaxWidth(end_graphic) + gui_def.HotBarEndOffset.X;
                    hot_bar_pos.Y = hot_bar_pos.Y + gui_def.HotBarEndOffset.Y;
                    CEnginePrimitive2DSprite hot_bar_end_sprite(0, GFToScreen(hot_bar_pos), (EEngine2DLayer)0x46, end_graphic, 0,
                        ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha((unsigned char)HUDRingAlpha), 0xff,
                        0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                    info.Engine.SetPersistentPrimitive(HotBarPrimitiveHandles[1], hot_bar_end_sprite, 0xc0);
                }
            }
        } else {
            HotBarPrimitiveHandles[0].Clear();
            HotBarPrimitiveHandles[1].Clear();
        }
    }
}

}

int main()
{
    using namespace NPlayerGui;
    CIEngine engine;
    CEnginePrimitivePrepareInfo info(engine);
    CPlayerGui& gui=CPlayerGui::Get();

    CDrawQuickAccessIconsPC no_hero;
    no_hero.Draw(info);
    assert(no_hero.bases==1 && !no_hero.drawing && no_hero.HotBarPrimitiveHandles[0].graphic==-2);

    for (unsigned awake=0; awake<2; ++awake) {
        for (unsigned mask=0; mask<16; ++mask) {
            for (unsigned cap=0; cap<2; ++cap) {
                CDrawQuickAccessIconsPC draw;
                CThingPlayerCreature hero;
                draw.player.hero=&hero;
                hero.alive=awake!=0;
                CPlayerGuiDef& def=gui.gui_def;
                def.DPadRingsPos=C2DVector(10.25f,-1.25f);
                def.HotBarOffset=C2DVector(2.0f,3.5f);
                def.HotBarEndOffset=C2DVector(1.0f,-8.0f);
                def.HotBarSingleGraphic=10;
                def.HotBarMainGraphic=11;
                gui.graphic_bank.width_reads=0;
                long last=-1;
                for (long i=0; i<4; ++i) {
                    const bool used=(mask & (1U<<i))!=0;
                    hero.inventory.graphics[i]=used ? i*10+5 : (i&1 ? -1 : 0);
                    if (used) last=i;
                    def.QuickAccessItemButtons[i].Offset=C2DVector(40.0f*i,-12.5f*i);
                    if (i<3) def.HotBarEndGraphics[i]=cap ? 21+i : 0;
                }
                draw.Draw(info);
                const CEnginePrimitiveHandle& bar=draw.HotBarPrimitiveHandles[0];
                const CEnginePrimitiveHandle& end=draw.HotBarPrimitiveHandles[1];
                assert(draw.bases==1 && hero.inventory.reads==(awake ? 4U : 0U));
                assert(draw.drawing==(awake && last>=0));
                assert(gui.graphic_bank.width_reads==(awake && last>0 && cap ? 1U : 0U));
                if (!awake) {
                    assert(bar.graphic==-2 && end.graphic==-2);
                } else if (last<0) {
                    assert(bar.clears==1 && end.clears==1);
                } else {
                    const float x=12.25f+40.0f*last;
                    const float y=2.25f-12.5f*last;
                    assert(bar.graphic==(last==0 ? 10 : 11));
                    assert(bar.pos.X==x*2.0f && bar.pos.Y==y*4.0f);
                    assert(bar.updates==1 && bar.alpha==68);
                    if (last==0)
                        assert(end.clears==1);
                    else if (cap) {
                        assert(end.updates==1 && end.graphic==20+last);
                        assert(end.pos.X==(x-15.0f)*2.0f && end.pos.Y==(y-8.0f)*4.0f && end.alpha==68);
                    } else
                        assert(end.graphic==-2 && end.clears==0);
                }
            }
        }
    }
    std::puts("00657580_TEST PASS");
}