// 0x00646F67 NPlayerGui::CDrawExperience::Draw -- draw gained experience with an icon, text and sized bar.
// Inline-budget: /d2inlS20 expands the original colour copy at006470aa; S15 calls it.
// Leaf: ?Draw@CDrawExperience@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: 0x00646F67,843bytes,thiscall.
// Flags: /d2inlS20
#ifdef _MSC_VER
#pragma optimize("y", off)
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_NPLAYERGUI_CSTATBAR_CHANGE_TEXT_PASS23
#define FABLE_RETAIL_NPLAYERGUI_CMULTISTATBAR_PASS23
#define FABLE_RETAIL_NPLAYERGUI_CDRAWEXPERIENCE_PASS23
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_GUIMODE
#define FABLE_RETAIL_CPLAYERGUI_DRAWTEXT
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_EXPERIENCE_PASS23
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_CGRAPHICDATABANK_FRAMESIZE
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_BASE
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_BOX_PASS23
#define FABLE_RETAIL_CFONTBANK_STRING_HELPERS
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CIEngine.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CFontBank.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/NPlayerGui_CMultiStatBar.h"
#include "engine/NPlayerGui_CDrawExperience.h"
#include "engine/NPlayerGui_Graphics.h"

CWideString FABLE_FASTCALL GFIntToWideString(long value);               // 0x00405F00
void FABLE_FASTCALL GFACoordToVCoord(const C2DVector& in, C2DVector& out);  // 0x009E21D0

void NPlayerGui::CDrawExperience::Draw(const CEnginePrimitivePrepareInfo& info)
{
    EStatUpdateType type = PlayerGui.PStatUpdates->GetCurrentType();
    if (type < 0 || type > 3)
        return;
    SetAsDrawing(true);
    const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
    unsigned char alpha = PlayerGui.PStatUpdates->GetFadeAlpha();
    C2DVector exp_pos = gui_def.ExperiencePos;
    long exp_graphic = gui_def.ExperienceGraphic;
    switch (type) {
    case 0: exp_graphic = gui_def.ExperienceStrengthGraphic; break;
    case 1: exp_graphic = gui_def.ExperienceSkillGraphic; break;
    case 2: exp_graphic = gui_def.ExperienceWillGraphic; break;
    case 3: exp_graphic = gui_def.ExperienceGeneralGraphic; break;
    }
    CEnginePrimitive2DSprite exp_sprite(0, GFToScreen(exp_pos), ENGINE_2D_LAYER_1, exp_graphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0,
                                  0.0f, 0.0f, ScaleAlpha(alpha), 255, 255, 255, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[0], exp_sprite, 0xc0);
    const CFontBank& font = Component.PeekMainFont();
    CWideString exp_text = GFIntToWideString(PlayerGui.PStatUpdates->GetCurrentChange());
    CRGBColour colour_with_alpha = gui_def.ExperienceTextColour;
    colour_with_alpha.A = (unsigned char)(colour_with_alpha.A * PlayerGui.Alpha);
    C2DVector vtext_pos;
    GFACoordToVCoord(exp_pos + gui_def.ExperienceTextOffset, vtext_pos);
    CPlayerGui::Get().Draw2DText(vtext_pos, exp_text, font, colour_with_alpha, 0.85f, ENGINE_2D_LAYER_2, false);
    float box_height = (float)Bank.GetGraphicFrameHeight(gui_def.ExperienceMiddleGraphic, 0);
    float box_width = font.GetStringWidth(exp_text) + gui_def.ExperienceMiddleMinWidth;
    C2DVector bar_pos = exp_pos + gui_def.ExperienceMiddleOffset;
    C2DBoxF bar_box(bar_pos, C2DExtentsF(box_width, box_height));
    CEnginePrimitive2DSprite bar_sprite(0, GFToScreen(bar_box), ENGINE_2D_LAYER_4, gui_def.ExperienceMiddleGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
                                    ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(alpha), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[1], bar_sprite, 0xc0);
    // Fidelity: original donor020e386b updates this position; PAREN settles native0064721b scheduling.
    bar_pos.X += (bar_box.GetWidth());
    CEnginePrimitive2DSprite bar_end_sprite(0, GFToScreen(bar_pos), ENGINE_2D_LAYER_4, gui_def.ExperienceEndGraphic, 0,
                                 ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(alpha), 255, 255, 255, 2,
                                 C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[2], bar_end_sprite, 0xc0);
}