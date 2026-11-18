// NPlayerGui::CDrawQuickAccessIconsPC::Draw(const CEnginePrimitivePrepareInfo& info) -- draw the base quick-access icons, then the PC hot bar behind the used quick-access slots: the single-slot graphic, or the main bar up to the last used slot plus its end cap
// Grade: DIFFER(1057v1057) -- one x87 operand-order site: branch 1 Y sum 0x00657663 (retail fld [anchor.Y] ; fadd [offset.Y], ours fld [offset.Y] ; fadd [anchor.Y]); the alpha byte load (xor edx,edx ; mov dl,[HUDRingAlpha]) follows. Frame, registers, calls, Clear expansions and branch 2 match.
// Leaf: ?Draw@CDrawQuickAccessIconsPC@NPlayerGui@@UAEXABVCEnginePrimitivePrepareInfo@@@Z
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_GRAPHICBANK
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_VSLOTS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_GETPLAYER_CALLED
#define FABLE_RETAIL_NPLAYERGUI_ANCHORS
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_CALLED
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_QUICKACCESSBUTTONS
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/NPlayerGui_Graphics.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerGui.h"
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/NPlayerGui_CDrawQuickAccessIconsPC.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/ThingAlive.h"
#include "engine/CPlayer.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CTCInventory.h"

extern long HUDRingAlpha;                                             // 0x01375744 (descriptive, as the landed 0x00652FA4)

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
                C2DVector hot_bar_pos = GFAnchorToBottomLeft(gui_def.DPadRingsPos) + gui_def.QuickAccessItemButtons[0].Offset + gui_def.HotBarOffset;
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
