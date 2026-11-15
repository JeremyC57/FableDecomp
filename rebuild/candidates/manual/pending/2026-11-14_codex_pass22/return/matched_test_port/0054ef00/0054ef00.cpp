// 0054EF00 NUISystem::CText::Draw -- render text and its bottom-right shadow, or clear off-screen handles.
// Inline-budget: original Clear sites score87/79(default74), measuredT88 admits all4.
// Retail:0x0054EF00,1127bytes,thiscall ret20.
// Leaf: ?Draw@CText@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z
// Flags: /d2inlT88
// Retail-asm: 0x0054F0B0 `fistp dword ptr [esp+28h]` (and 0x0054F0C0, the y rounding) -- the engine's inline-asm float-to-long
//   helper GFFloatToLongNear (shared engine/GFFloatToLongNear.h); VC7.1 emits no bare dword fistp from C++ here ((long)x -> __ftol2,
//   /QIfist -> fistp qword. ROUNDING_LOWERING_CATALOG.json records 30 C++ and 10 intrinsic forms at both sites.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTEXT
#define FABLE_RETAIL_CTEXT_LAYOUT
#define FABLE_RETAIL_CTEXT_DRAW_PRIMITIVE_BODIES
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/CText.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitive2DText.h"
#include "engine/GFFloatToLongNear.h"
// Original complete descriptor bodies; native114-byte ctor and91-byte assignment are audited separately.
CEnginePrimitive2DText::CEnginePrimitive2DText(unsigned __int64 id, const C2DVector& position,
    EEngine2DLayer layer, float scale, const CWideString& text, const CFontBank& font,
    const CRGBColour& colour, EEngineInterpolationSet interpolation_set)
    : CEnginePrimitive((EEnginePrimitiveType)0x27, id, interpolation_set),
      Pos(position.X, position.Y, 0.0f), Layer(layer), Scale(scale), Colour(colour), Text(text), PFont(&font)
{
}
CEnginePrimitive2DText& CEnginePrimitive2DText::operator=(const CEnginePrimitive2DText& other)
{
    CEnginePrimitive::operator=(other);
    Pos = other.Pos;
    Layer = other.Layer;
    Scale = other.Scale;
    Colour = other.Colour;
    Text = other.Text;
    PFont = other.PFont;
    return *this;
}

namespace NUISystem {
void CText::Draw(CIEngine& engine, CEnginePrimitiveHandle* p_handle, long layer, long* pprimitive_index, CComponent* pparent)
{
    if (RenderColour.A <= 0 || RenderZoom.X <= 0.0f) {
        if (NoRenderNextFrame == 1) {
            if (p_handle) {
                engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
                engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
            } else {
                Handle.Clear();
                HandleDropShadowBottomRight.Clear();
            }
            return;
        }
        NoRenderNextFrame = 1;
    } else
        NoRenderNextFrame = 0;
    CRGBColour colour = RenderColour;
    C2DVector render_position = RelativeRenderPosition;
    float render_zoom_x = ScaleText ? RelativeRenderZoom.X : RenderZoom.X;
    if (render_position.X >= CUIScreenUnitConverter::GetBackBufferWidth() || render_position.Y >= CUIScreenUnitConverter::GetBackBufferHeight()) {
        if (p_handle) {
            engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
            engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
        } else {
            Handle.Clear();
            HandleDropShadowBottomRight.Clear();
        }
        return;
    }
    bool is_independent = IsIndependent();
    bool is_layerindependent = IsLayerIndependent();
    long render_layer;
    if (!is_independent && !is_layerindependent)
        render_layer = Layer + layer;
    else
        render_layer = Layer;
    CRGBColour black(0, 0, 0, colour.A);
    CEnginePrimitive2DText primitive;
    CEnginePrimitive2DText primitive_bottom_right;
    switch (GetAlignment()) {
    case CENTER:
        render_position.X -= render_zoom_x * Size.X * 0.5f;
        break;
    case RIGHT:
        render_position.X -= render_zoom_x * Size.X;
        break;
    }
    render_position.X = (float)GFFloatToLongNear(render_position.X);
    render_position.Y = (float)GFFloatToLongNear(render_position.Y);
    if (Unavailable) {
        primitive = CEnginePrimitive2DText(0, render_position, CManager::GetInstance()->GetLayer(render_layer), render_zoom_x, Value, *PFont, black, (EEngineInterpolationSet)0);
        C2DVector position(render_position.X + 2.0f, render_position.Y + 2.0f);
        primitive_bottom_right = CEnginePrimitive2DText(0, position, CManager::GetInstance()->GetLayer(render_layer + 1), render_zoom_x, Value, *PFont, colour, (EEngineInterpolationSet)0);
    } else {
        primitive = CEnginePrimitive2DText(0, render_position, CManager::GetInstance()->GetLayer(render_layer), render_zoom_x, Value, *PFont, colour, (EEngineInterpolationSet)0);
        C2DVector position(render_position.X + 2.0f, render_position.Y + 2.0f);
        primitive_bottom_right = CEnginePrimitive2DText(0, position, CManager::GetInstance()->GetLayer(render_layer + 1), render_zoom_x, Value, *PFont, black, (EEngineInterpolationSet)0);
    }
    if (p_handle && pprimitive_index) {
        engine.SetChildPrimitive(*p_handle, (*pprimitive_index)++, primitive, 0x40);
        engine.SetChildPrimitive(*p_handle, (*pprimitive_index)++, primitive_bottom_right, 0x40);
    } else {
        engine.SetPersistentPrimitive(Handle, primitive, 0x40);
        engine.SetPersistentPrimitive(HandleDropShadowBottomRight, primitive_bottom_right, 0x40);
    }
    return;
}
}
