// 0064d7a6 NPlayerGui::CDrawQuickAccessIcons::DrawChargeGraphic
// Leaf: ?DrawChargeGraphic@CDrawQuickAccessIcons@NPlayerGui@@AAEXABVCEnginePrimitivePrepareInfo@@ABVC2DVector@@J@Z
// Flags: /GS
#ifdef _MSC_VER
#pragma optimize("y",off)
#pragma optimize("s",on)
#endif
#define FABLE_RETAIL_NPLAYERGUI_CHARGE_GRAPHIC_PASS26
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
// Inline-budget: native clock sprite copies both colour objects in place at 006559BB and 006559CB.
#define FABLE_RETAIL_CRGBCOLOUR_CURSOR_COPY_INLINE_PASS22
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
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CCOUNTEDPOINTER
#include "stlport/fable_containers.h"
// Fidelity: scaffolding for the normal /Oy STLport owner at 0x0040F220.
// The selected row restores its frame-pointer pragma before the caller.
#pragma optimize("y",on)
template<> inline const long& std::vector<long>::operator[](size_type n) const
{
    return *(begin()+n);
}
#pragma optimize("y",off)

#include "fable_string.h"
#include "engine/C2DVector.h"
// Fidelity: use the 004371A0 corners-owner frame mode, then restore the caller.
#pragma optimize("y",on)
#include "engine/C2DBoxF.h"
#pragma optimize("y",off)
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/NPlayerGui_Graphics.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerGui.h"
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/NPlayerGui_CDrawQuickAccessIcons.h"
#include "engine/CEnginePrimitive2DClockSprite.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/ThingAlive.h"
#include "engine/CPlayer.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CTCInventory.h"


namespace NPlayerGui {
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
unsigned char CDrawBase::ScaleAlpha(unsigned char alpha) const { return PlayerGui->ScaleAlpha(alpha); }
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
void CDrawQuickAccessIcons::DrawChargeGraphic(const CEnginePrimitivePrepareInfo& info,
                                             const C2DVector& button_offset, long button_index)
{
    const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
    float angle = 0.0f;
    if (FullyCharged) angle = 1.0f;
    else {
        float activation_time = GetActivationTimeForButton(ChargingButtonIndex);
        if (activation_time > DELTA_NOTIONAL_ZERO && activation_time > ChargingButtonTime)
            angle = ChargingButtonTime / activation_time;
    }
    if (angle > DELTA_NOTIONAL_ZERO) {
        long index = button_index % (long)gui_def.AbilityButtons.size();
        if (button_index == -2) index = 0; else index = 1;
        if (angle > 1.0f) angle = 1.0f;
        angle = angle / 2.0f + gui_def.DPadRingsChargeAngles[index];
        C2DVector charge_pos = GFAnchorToBottomLeft(gui_def.DPadRingsPos) + button_offset + gui_def.DPadRingsChargeOffsets[index];
        CEnginePrimitive2DClockSprite charge_sprite(0, charge_pos, ENGINE_2D_LAYER_1,
            gui_def.DPadRingsChargeGraphics[index], 0, CRGBColour(0,0,0,ScaleAlpha(255)),
            gui_def.DPadRingsChargeGraphics[index], 0, CRGBColour(255,255,255,ScaleAlpha(255)),
            ENGINE_PRIMITIVE_BLEND_ALPHA, angle, angle, C2DVector(0,0), C2DVector(0,0), 2, (EEngineInterpolationSet)0);
        info.Engine.SetPersistentPrimitive(ChargeUpPrimitiveHandle, charge_sprite, 0xc0);
    }
}
}