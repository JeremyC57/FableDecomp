// 00644677 NPlayerGui::CDrawLetterBoxBlackBars::Draw
// Leaf: ?Draw@CDrawLetterBoxBlackBars@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags:
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DRAW_LETTERBOX_PASS29
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_BOX_PASS23
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKDISPLAYENGINE

#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/C2DExtentsF.h"
#include "engine/NPlayerGui_CDrawLetterBoxBlackBars.h"
#include "engine/NPlayerGui_Graphics.h"
#include "engine/GFGetInterpolatedValue.h"

extern double VIRTUAL_SCREEN_WIDTH;
extern double VIRTUAL_SCREEN_HEIGHT;
float FABLE_FASTCALL GFVXToAX(float x);
float FABLE_FASTCALL GFVYToAY(float y);

#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
namespace NPlayerGui {
void CDrawLetterBoxBlackBars::Draw(const CEnginePrimitivePrepareInfo& info)
{
    SetAsDrawing(true);
    const CDisplayEngine& display_engine = Component.PeekDisplayEngine();
    const float screen_ratio = display_engine.GetLetterBoxRatio();
    const float alpha = display_engine.GetLetterBoxStrength();
    const float full_screen_height = GFVYToAY(static_cast<float>(VIRTUAL_SCREEN_HEIGHT));
    const float final_screen_height = GFVXToAX(static_cast<float>(VIRTUAL_SCREEN_WIDTH)) / screen_ratio;
    const float current_screen_height = GFGetInterpolatedValue(full_screen_height, final_screen_height, alpha);
    const float height_of_black_bars = (full_screen_height - current_screen_height) * 0.5f;

    C2DBoxF top_bar_area(C2DVector(-8.0f, -8.0f),
        C2DExtentsF(GFVXToAX(static_cast<float>(VIRTUAL_SCREEN_WIDTH)) + 8.0f, height_of_black_bars));
    CEnginePrimitive2DSprite top_bar(0, GFToScreen(top_bar_area), ENGINE_2D_LAYER_12,
        0, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT, 0.0f, 0.0f,
        255, 0, 0, 0, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[0], top_bar, 0x40);

    C2DBoxF bottom_bar_area(C2DVector(-8.0f, GFVYToAY(static_cast<float>(VIRTUAL_SCREEN_HEIGHT)) - height_of_black_bars),
        C2DExtentsF(GFVXToAX(static_cast<float>(VIRTUAL_SCREEN_WIDTH)) + 8.0f, height_of_black_bars + 8.0f));
    CEnginePrimitive2DSprite bottom_bar(0, GFToScreen(bottom_bar_area), ENGINE_2D_LAYER_12,
        0, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT, 0.0f, 0.0f,
        255, 0, 0, 0, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[1], bottom_bar, 0x40);
}
}