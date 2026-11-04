// NUISystem::CText::Draw(CIEngine& engine, CEnginePrimitiveHandle* parentHandle, long layer, long* childIndex, CComponent* parent) -- draw the text and its bottom-right drop shadow as two 2D text primitives (child primitives of the parent handle, or the component's own persistent handles), or clear them when invisible or off screen
// Grade: DIFFER(1127v1127) -- residue: frame slot order of the five small locals (retail position 0x10, scale 0x18, bool 0x1f, colour 0x20, shadowColour 0x24; ours colour 0x10, shadowColour 0x14, position 0x18, scale 0x20, bool 0x27) and one ecx/edx swap in the 2D-text ctor argument setup; 53 diff lines, 19 grades
// Leaf: ?Draw@CText@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z
// Flags: none
// Retail-asm: 0x0054F0B0 `fistp dword ptr [esp+28h]` (and 0x0054F0C0, the y rounding) -- the engine's inline-asm float-to-long
//   helper GFFloatToLongNear (shared engine/GFFloatToLongNear.h); VC7.1 emits no bare dword fistp from C++ here ((long)x -> __ftol2,
//   /QIfist -> fistp qword; both compiled at this site, NOTES.md "C++ attempts before helper").
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTEXT
#define FABLE_RETAIL_CTEXT_LAYOUT
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/CText.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitive2DText.h"
#include "engine/GFFloatToLongNear.h"
namespace NUISystem {
void CText::Draw(CIEngine& engine, CEnginePrimitiveHandle* parentHandle, long layer, long* childIndex, CComponent* parent)
{
    if (RenderColour.A <= 0 || RenderZoom.X <= 0.0f) {
        if (NoRenderNextFrame == 1)
            goto release;
        NoRenderNextFrame = 1;
    } else
        NoRenderNextFrame = 0;
    {
    CRGBColour colour = RenderColour;
    C2DVector position = RelativeRenderPosition;
    float scale = ScaleText ? RelativeRenderZoom.X : RenderZoom.X;
    if (position.X >= CUIScreenUnitConverter::GetBackBufferWidth() || position.Y >= CUIScreenUnitConverter::GetBackBufferHeight())
        goto release;
    {
        bool independent = IsIndependent();
        bool layerIndependent = IsLayerIndependent();
        long drawLayer;
        if (!independent && !layerIndependent)
            drawLayer = Layer + layer;
        else
            drawLayer = Layer;
        CRGBColour shadowColour(0, 0, 0, colour.A);
        CEnginePrimitive2DText text;
        CEnginePrimitive2DText shadow;
        switch (GetAlignment()) {
        case CENTER:
            position.X -= scale * Size.X * 0.5f;
            break;
        case RIGHT:
            position.X -= scale * Size.X;
            break;
        }
        position.X = (float)GFFloatToLongNear(position.X);
        position.Y = (float)GFFloatToLongNear(position.Y);
        if (Unavailable) {
            text = CEnginePrimitive2DText(0, position, CManager::GetInstance()->GetLayer(drawLayer), scale, Value, *PFont.Data, shadowColour, (EEngineInterpolationSet)0);
            C2DVector shadowPosition(position.X + 2.0f, position.Y + 2.0f);
            shadow = CEnginePrimitive2DText(0, shadowPosition, CManager::GetInstance()->GetLayer(drawLayer + 1), scale, Value, *PFont.Data, colour, (EEngineInterpolationSet)0);
        } else {
            text = CEnginePrimitive2DText(0, position, CManager::GetInstance()->GetLayer(drawLayer), scale, Value, *PFont.Data, colour, (EEngineInterpolationSet)0);
            C2DVector shadowPosition(position.X + 2.0f, position.Y + 2.0f);
            shadow = CEnginePrimitive2DText(0, shadowPosition, CManager::GetInstance()->GetLayer(drawLayer + 1), scale, Value, *PFont.Data, shadowColour, (EEngineInterpolationSet)0);
        }
        if (parentHandle && childIndex) {
            engine.SetChildPrimitive(*parentHandle, (*childIndex)++, text, 0x40);
            engine.SetChildPrimitive(*parentHandle, (*childIndex)++, shadow, 0x40);
        } else {
            engine.SetPersistentPrimitive(Handle, text, 0x40);
            engine.SetPersistentPrimitive(HandleDropShadowBottomRight, shadow, 0x40);
        }
    }
    }
    return;
release:
    if (parentHandle) {
        engine.ClearChildPrimitive(*parentHandle, (*childIndex)++);
        engine.ClearChildPrimitive(*parentHandle, (*childIndex)++);
    } else {
        Handle.Clear();
        HandleDropShadowBottomRight.Clear();
    }
}
}
