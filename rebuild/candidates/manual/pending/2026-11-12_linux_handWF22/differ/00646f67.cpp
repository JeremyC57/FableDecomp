// NPlayerGui::CDrawExperience::Draw(const CEnginePrimitivePrepareInfo&) -- draw the experience-gain readout: the def's icon for the stat-updates bar's current type, the current change as text, a middle box sized from the text width and an end cap
// Grade: DIFFER(843v843) -- frame, slots and every other instruction match; only the end-cap sprite argument setup (0x0064721B..0x00647250) is scheduled differently: retail computes the end X (fld [-50h]; fsub [-8]; fadd [-8]) BEFORE the two zero uv vectors and copies Y (mov eax,[-4] / mov [-34h],eax) right after the first fldz; ours builds the uv zeros first and computes X after them (7 hunks).
// Retail 0x00646F67, 843 bytes, __thiscall ret 4, frame pointer (optimize y off + s on).
// Callees: CMultiStatBar::GetCurrentType 0x00646D90, CPlayerGui::PeekGuiDef 0x00438570, _ftol2 0x00BFEA70, CDrawBase::ScaleAlpha 0x00643D5D,
//   CEnginePrimitive2DSprite ctors 0x00493AC2 (C2DVector) / 0x0041BEB0 (C2DBoxF), CIEngine::SetPersistentPrimitive (slot 23),
//   CMultiStatBar::GetCurrentChange 0x00646DA7, GFIntToWideString 0x00405F00, GFACoordToVCoord 0x009E21D0, CPlayerGui::Draw2DText 0x00438010,
//   CGraphicDataBank::GetGraphicFrameHeight (slot 22), CFontBank::GetStringWidth 0x0048DA62, CWideString::~CWideString 0x0099B510.
// Leaf: ?Draw@CDrawExperience@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_COPY_FORCEINLINE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_TRPOS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_CPLAYERGUI_DRAWTEXT
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_FRIEND_EXPERIENCE
#define FABLE_RETAIL_CPLAYERGUIDEF_EXPERIENCE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_CGRAPHICDATABANK_FRAMESIZE
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_BOX
#define FABLE_RETAIL_CFONTBANK_STRING_HELPERS
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CFontBank.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/NPlayerGui_CStatBar.h"
#include "engine/NPlayerGui_Graphics.h"

enum EStatUpdateType { };
namespace NPlayerGui {
class CMultiStatBar : public CStatBar {
public:
    EStatUpdateType GetCurrentType() const;                 // 0x00646D90
    long GetCurrentChange() const;                          // 0x00646DA7
    unsigned char GetFadeAlpha() const { return (unsigned char)(FadeInTimer * 255.0f); }
};
class CDrawExperience : public CDrawBase {
public:
    void Draw(const CEnginePrimitivePrepareInfo& info);
};
}
CWideString __fastcall GFIntToWideString(long value);               // 0x00405F00
void __fastcall GFACoordToVCoord(const C2DVector& in, C2DVector& out);  // 0x009E21D0

void NPlayerGui::CDrawExperience::Draw(const CEnginePrimitivePrepareInfo& info)
{
    EStatUpdateType type = PlayerGui->PStatUpdates->GetCurrentType();
    if (type < 0 || type > 3)
        return;
    SetAsDrawing(true);
    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
    unsigned char alpha = PlayerGui->PStatUpdates->GetFadeAlpha();
    C2DVector pos = def.ExperiencePos;
    long graphic = def.ExperienceGraphic;
    switch (type) {
    case 0: graphic = def.ExperienceStrengthGraphic; break;
    case 1: graphic = def.ExperienceSkillGraphic; break;
    case 2: graphic = def.ExperienceWillGraphic; break;
    case 3: graphic = def.ExperienceGeneralGraphic; break;
    }
    CEnginePrimitive2DSprite icon(0, GFToScreen(pos), ENGINE_2D_LAYER_1, graphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0,
                                  0.0f, 0.0f, ScaleAlpha(alpha), 255, 255, 255, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[0], icon, 0xc0);
    const CFontBank& font = Component->PeekMainFont();
    CWideString text = GFIntToWideString(PlayerGui->PStatUpdates->GetCurrentChange());
    CRGBColour colour = def.ExperienceTextColour;
    colour.A = (unsigned char)(colour.A * PlayerGui->Alpha);
    C2DVector screenPos;
    GFACoordToVCoord(pos + def.ExperienceTextOffset, screenPos);
    CPlayerGui::Get().Draw2DText(screenPos, text, font, colour, 0.85f, ENGINE_2D_LAYER_2, false);
    float height = (float)Bank->GetGraphicFrameHeight(def.ExperienceMiddleGraphic, 0);
    float width = font.GetStringWidth(text) + def.ExperienceMiddleMinWidth;
    C2DVector middlePos = pos + def.ExperienceMiddleOffset;
    C2DBoxF box(middlePos, C2DExtentsF(width, height));
    CEnginePrimitive2DSprite middle(0, GFToScreen(box), ENGINE_2D_LAYER_4, def.ExperienceMiddleGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
                                    ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(alpha), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[1], middle, 0xc0);
    CEnginePrimitive2DSprite end(0, GFToScreen(box.GetTRPos()), ENGINE_2D_LAYER_4, def.ExperienceEndGraphic, 0,
                                 ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(alpha), 255, 255, 255, 2,
                                 C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[2], end, 0xc0);
}
