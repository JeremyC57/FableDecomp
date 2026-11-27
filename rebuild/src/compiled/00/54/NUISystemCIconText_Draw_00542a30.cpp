// 00542a30 NUISystem::CIconText::Draw
// The shared enum and Ego_d identify 0x40 as EPF_2D and 0xC0 as EPF_2D | EPF_BLOCK_ON_LOAD.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_C2DVECTOR_ORIGINAL_COPIES_PASS26
#define FABLE_RETAIL_CTEXT_DRAW_PRIMITIVE_BODIES
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_LAYER_ENUM_FROM_CMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_BOX_CTOR_BODY
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_C2DBOXF_COPY
// Leaf: ?Draw@CIconText@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z
// Flags: /Gr
// Retail-asm: 0x00542C97 `fistp dword ptr [esp+28h]` (and 0x00542CA7, the y rounding) -- the engine's inline-asm float-to-long
//   helper GFFloatToLongNear (shared engine/GFFloatToLongNear.h, Retail-asm-helper); no C++ form gives the bare dword fistp.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#ifdef _MSC_VER
// Inline-budget: retail 0041cc14 selects the size-built screen width accessor.
#pragma optimize("s",on)
#endif
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#include "engine/EEnginePrimitiveType.h"
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CIconText.h"
#include "engine/NUISystem_CManager.h"

#include "engine/CIEngine.h"
#include "engine/CEnginePrimitive2DText.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/GFFloatToLongNear.h"
#define FABLE_RETAIL_STLP_MEM_FUN_REF_ORIGINAL_PASS22
#include "stlport/fable_algorithm.h"
namespace NUISystem {
void CIconText::Draw(CIEngine& engine, CEnginePrimitiveHandle* p_handle, long layer, long* pprimitive_index, CComponent* pparent)
{
    if (RenderColour.A <= 0 || RenderZoom.X <= 0.0f) {
        if (NoRenderNextFrame == 1) {
            if (p_handle) {
                engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
                engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
            } else {
                do {
                    TextHandle.Clear();
                    DropShadowHandle.Clear();
                    std::for_each(IconHandles.begin(), IconHandles.end(), std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
                } while (0);
            }
            return;
        }
        NoRenderNextFrame = 1;
    } else
        NoRenderNextFrame = 0;
    CRGBColour colour = RenderColour;
    C2DVector render_position = RelativeRenderPosition;
    if (render_position.X >= CUIScreenUnitConverter::GetBackBufferWidth() || render_position.Y >= CUIScreenUnitConverter::GetBackBufferWidth()) {
        do {
            TextHandle.Clear();
            DropShadowHandle.Clear();
            std::for_each(IconHandles.begin(), IconHandles.end(), std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
        } while (0);
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
        render_position.X -= Size.X * 0.5f;
        break;
    case RIGHT:
        render_position.X -= Size.X;
        break;
    }
    render_position.X = (float)GFFloatToLongNear(render_position.X);
    render_position.Y = (float)GFFloatToLongNear(render_position.Y);
    C2DVector render_zoom = RelativeRenderZoom;
    if (Unavailable) {
        primitive = CEnginePrimitive2DText(0, render_position, CManager::GetInstance()->GetLayer(render_layer), render_zoom.X, DisplayText, *PFont, black, ENGINE_INTERPOLATION_SET_DEFAULT);
        C2DVector position(render_position);
        position.X += 2.0f;
        position.Y += 2.0f;
        primitive_bottom_right = CEnginePrimitive2DText(0, position, CManager::GetInstance()->GetLayer(render_layer + 1), render_zoom.X, DisplayText, *PFont, colour, ENGINE_INTERPOLATION_SET_DEFAULT);
    } else {
        primitive = CEnginePrimitive2DText(0, render_position, CManager::GetInstance()->GetLayer(render_layer), render_zoom.X, DisplayText, *PFont, colour, ENGINE_INTERPOLATION_SET_DEFAULT);
        C2DVector position(render_position);
        position.X += 2.0f;
        position.Y += 2.0f;
        primitive_bottom_right = CEnginePrimitive2DText(0, position, CManager::GetInstance()->GetLayer(render_layer + 1), render_zoom.X, DisplayText, *PFont, black, ENGINE_INTERPOLATION_SET_DEFAULT);
    }
    if (p_handle && pprimitive_index) {
        engine.SetChildPrimitive(*p_handle, (*pprimitive_index)++, primitive, 0x40);
        engine.SetChildPrimitive(*p_handle, (*pprimitive_index)++, primitive_bottom_right, 0x40);
    } else {
        engine.SetPersistentPrimitive(TextHandle, primitive, 0x40);
        engine.SetPersistentPrimitive(DropShadowHandle, primitive_bottom_right, 0x40);
        IconHandles.resize(Icons.size());
        for (long index = 0; index < Icons.size(); ++index) {
            CEmbeddedIcon& icon = Icons[index];
            C2DVector position = render_position + icon.Offset;
            C2DExtentsF range(icon.Size.X * render_zoom.X, icon.Size.Y * render_zoom.Y);
            C2DBoxF clip_box(position, range);
            CEnginePrimitive2DSprite primitive_sprite(0, clip_box, CManager::GetInstance()->GetLayer(render_layer), icon.Graphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT, 0.0f, 0.0f,
                                            colour.A, colour.R, colour.G, colour.B, 2, C2DVector(0.0f,0.0f), C2DVector(0.0f,0.0f));
            engine.SetPersistentPrimitive(IconHandles[index], primitive_sprite, 0xC0);
        }
    }
}
}

CEnginePrimitive2DText::CEnginePrimitive2DText(unsigned __int64 uid, const C2DVector& pos,
    EEngine2DLayer layer, float scale, const CWideString& text, const CFontBank& font,
    const CRGBColour& colour, EEngineInterpolationSet interp_set)
    : CEnginePrimitive(ENGINE_PRIMITIVE_2D_TEXT, uid, interp_set), Pos(pos.X,pos.Y,0),
      Layer(layer), Scale(scale), Colour(colour), Text(text), PFont(&font) {}


CEnginePrimitive2DText& CEnginePrimitive2DText::operator=(const CEnginePrimitive2DText& __that)
{
    CEnginePrimitive::operator=(__that);
    Pos = __that.Pos;
    Layer = __that.Layer;
    Scale = __that.Scale;
    Colour = __that.Colour;
    Text = __that.Text;
    PFont = __that.PFont;
    return *this;
}