// NUISystem::CIconText::Draw(CIEngine& engine, CEnginePrimitiveHandle* parentHandle, long layer, long* childIndex, CComponent* parent) -- draw the display text and its bottom-right drop shadow as two 2D text primitives (child primitives of the parent handle, or the component's own persistent handles) and, on the persistent path, one 2D sprite per embedded icon; clear every handle when invisible or off screen
// Grade: DIFFER(1669v1714) -- residue: frame slot assignment. Retail keeps two unassigned 4-byte slots (0x1C, 0x24) around `colour` (0x20), so the Unavailable branch's shadow position reuses the dead shadow-colour slot 0x18 and `scale` reuses the rounding temporary's slot 0x28; ours packs colour at 0x1C, the temporary and both shadow positions at 0x24 and scale at 0x30 (frame 0xFC vs 0xF8). Everything downstream of that is the same code: the constant registers swap (retail ebx = 0x27 / ebp = 0, so the three theme-channel bytes are imm8 stores; ours ebx = 0 stores them from bl) and the cross-jumped tail of the two text-constructor branches starts earlier. 20 grades.
// Next idea: find the two 4-byte locals retail homes at 0x1C / 0x24 (not the IsIndependent bools, not the second rounding: probes o1 / o2 kept one hole at 0x20); try a `float` scale copy plus a separate C2DVector for the icon loop, or an `EEngine2DLayer` local per text primitive, and declare `colour` after the two bools (probe i1 moved it to 0x24).
// Leaf: ?Draw@CIconText@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z
// Flags: none
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
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CIconText.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitive2DText.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/GFFloatToLongNear.h"
namespace std {                                                          // STLport 4.6 (retail std): the landed dtor 0x005424B0 model
template <class _Ret, class _Tp> class mem_fun_ref_t {
public:
    explicit mem_fun_ref_t(_Ret (_Tp::*__pf)()) : _M_f(__pf) { }
    _Ret operator()(_Tp& __r) const { return (__r.*_M_f)(); }
private:
    _Ret (_Tp::*_M_f)();
};
template <class _Ret, class _Tp> inline mem_fun_ref_t<_Ret, _Tp> mem_fun_ref(_Ret (_Tp::*__f)()) { return mem_fun_ref_t<_Ret, _Tp>(__f); }
template <class _InputIter, class _Function> _Function __fastcall for_each(_InputIter __first, _InputIter __last, _Function __f);   // 0x0043F360
}
CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64, const C2DBoxF& box, long layer, unsigned long bankIndex,
    unsigned long frame, long blendMode, long interpolationSet, float startAngle, float endAngle, unsigned char a, unsigned char r,
    unsigned char g, unsigned char b, unsigned long flags, const C2DVector& uvStart, const C2DVector& uvEnd)
    : CEnginePrimitive((EEnginePrimitiveType)0x22, (EEngineInterpolationSet)interpolationSet)
{
    ClipBox = box; Layer = layer; BankIndex = bankIndex; Frame = frame; StartAngle = startAngle; EndAngle = endAngle;
    BlendMode = blendMode; Colour = CRGBColour(r, g, b, a); ClipBoxIsPos = false; Flags = flags; Texture = 0;
    UVOffsetsStart = uvStart; UVOffsetsEnd = uvEnd;
}
namespace NUISystem {
void CIconText::Draw(CIEngine& engine, CEnginePrimitiveHandle* parentHandle, long layer, long* childIndex, CComponent* parent)
{
    if (RenderColour.A <= 0 || RenderZoom.X <= 0.0f) {
        if (NoRenderNextFrame == 1) {
            if (parentHandle) {
                engine.ClearChildPrimitive(*parentHandle, (*childIndex)++);
                engine.ClearChildPrimitive(*parentHandle, (*childIndex)++);
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
    C2DVector position = RelativeRenderPosition;
    CRGBColour colour = RenderColour;
    if (position.X >= CUIScreenUnitConverter::GetBackBufferWidth() || position.Y >= CUIScreenUnitConverter::GetBackBufferWidth()) {
        do {
            TextHandle.Clear();
            DropShadowHandle.Clear();
            std::for_each(IconHandles.begin(), IconHandles.end(), std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
        } while (0);
        return;
    }
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
        position.X -= Size.X * 0.5f;
        break;
    case RIGHT:
        position.X -= Size.X;
        break;
    }
    position.X = (float)GFFloatToLongNear(position.X);
    position.Y = (float)GFFloatToLongNear(position.Y);
    C2DVector scale = RelativeRenderZoom;
    if (Unavailable) {
        text = CEnginePrimitive2DText(0, position, CManager::GetInstance()->GetLayer(drawLayer), scale.X, DisplayText, *PFont, shadowColour, (EEngineInterpolationSet)0);
        C2DVector shadowPosition(position.X + 2.0f, position.Y + 2.0f);
        shadow = CEnginePrimitive2DText(0, shadowPosition, CManager::GetInstance()->GetLayer(drawLayer + 1), scale.X, DisplayText, *PFont, colour, (EEngineInterpolationSet)0);
    } else {
        text = CEnginePrimitive2DText(0, position, CManager::GetInstance()->GetLayer(drawLayer), scale.X, DisplayText, *PFont, colour, (EEngineInterpolationSet)0);
        C2DVector shadowPosition(position.X + 2.0f, position.Y + 2.0f);
        shadow = CEnginePrimitive2DText(0, shadowPosition, CManager::GetInstance()->GetLayer(drawLayer + 1), scale.X, DisplayText, *PFont, shadowColour, (EEngineInterpolationSet)0);
    }
    if (parentHandle && childIndex) {
        engine.SetChildPrimitive(*parentHandle, (*childIndex)++, text, 0x40);
        engine.SetChildPrimitive(*parentHandle, (*childIndex)++, shadow, 0x40);
    } else {
        engine.SetPersistentPrimitive(TextHandle, text, 0x40);
        engine.SetPersistentPrimitive(DropShadowHandle, shadow, 0x40);
        IconHandles.resize(Icons.size());
        for (unsigned long i = 0; i < Icons.size(); ++i) {
            CEmbeddedIcon& icon = Icons[i];
            C2DVector uvEnd(0.0f, 0.0f);
            C2DVector uvStart(0.0f, 0.0f);
            C2DVector iconPosition = position + icon.Offset;
            C2DVector iconSize(scale.X * icon.Size.X, scale.Y * icon.Size.Y);
            C2DBoxF box(iconPosition.X, iconPosition.Y, iconPosition.X + iconSize.X, iconPosition.Y + iconSize.Y);
            CEnginePrimitive2DSprite sprite(0, box, CManager::GetInstance()->GetLayer(drawLayer), icon.Graphic, 0, 2, 0, 0.0f, 0.0f,
                                            colour.A, colour.R, colour.G, colour.B, 2, uvStart, uvEnd);
            engine.SetPersistentPrimitive(IconHandles[i], sprite, 0xC0);
        }
    }
}
}
