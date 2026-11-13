// NPlayerGui::CDrawDamage::Draw(const CEnginePrimitivePrepareInfo& info) -- draw every queued damage value as red digit sprites: clear and resize the primitive handles (four per value), then for each value anchor the digits bottom-right (or at the hero position plus a duration-scaled hero offset), fade the alpha by the remaining duration, and set one persistent 2D sprite per digit (1..4 digits, 22 apart, the first pulled left and clamped at the screen edge), three handles per value
// Leaf: ?Draw@CDrawDamage@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064ED4F, 2314 bytes, thiscall, biased frame (`push ebp ; lea ebp,[esp-74h] ;
//   sub esp,478h`, `add ebp,74h ; leave ; ret 4`). Extent checked: the CDrawPerceivers ctor ends with `ret 8` at 0x0064ED4C and the
//   CDrawQuestInfo ctor starts at 0x0064F659 right after this row's only `ret 4` (no cold code, no padding: an unaligned size TU).
// Identity (proven): Ego_r.pdb 0x00691CB4 / FableWin.pdb 0x020E690D ?Draw@CDrawDamage@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z;
//   the body walks CDrawDamage::DamageValues (+0x28, the list the landed Update 0x00644CE2 / AddDamageChange 0x006472B2 own) and reads
//   SDamageValue Duration (+8 in the node) / Change (+0xC) / Hero (+0x10) and CPlayerGuiDef DigitPos / DigitHeroPos / DigitHeroOffset /
//   DigitGraphics (+0x814 .. +0x82C).
// Callees: CPlayerGui::PeekGuiDef 0x00438570, std::for_each<CEnginePrimitiveHandle*, mem_fun_ref_t<void, CEnginePrimitiveHandle> >
//   0x0043F360 with CEnginePrimitiveHandle::Clear 0x0041BE70, vector<CEnginePrimitiveHandle>::resize(size_type) 0x00443F90,
//   NPlayerGui::GFAnchorToBottomRight 0x00643D0C, CDrawBase::ScaleAlpha 0x00643D5D, __ftol2 0x00BFEA70,
//   CEnginePrimitive2DSprite(u64, const C2DVector&, EEngine2DLayer, ..) 0x00493AC2, CIEngine::SetPersistentPrimitive (slot 23, +0x5C).
//   Inline: GFAnchorToTopLeft (retail's uncalled out-of-line copy 0x00643C94), GFToScreen (copy 0x00643BE2), STLport list / vector.
// Globals: NPlayerGui::VALUE_DISPLAY_DURATION 0x013BA860 (landed initialiser 0x0121DF1C). Float pool 0, 10, 100, 1000, 10000, 11, 22,
//   33, 128, 255.
// Shape: size TU (`optimize("s", on)` alone gives the biased frame). CDrawBase::ScaleAlpha is a same-TU function in retail (0x00643D5D,
//   before this row): its body visible here (the landed row's bytes, out of line: callee size > 15 in a size TU) lets VC7.1 see it
//   writes no memory, so the inline DigitGraphics[..] lookups that c1 evaluates ahead of the sprite constructor's pushes are scheduled
//   after the ScaleAlpha call, as in retail (declaration only: the vector start is loaded before the call at every digit). A named
//   `long duration` gives retail's reload-and-compare (`mov eax,[ebp+54h] ; cmp eax,ebx ; lea eax,[eax+eax*4]`) while the 8-bit arm
//   re-reads it->Duration (`mov al,[ecx+8] ; imul dl`); the named float time stays on the x87 stack for the offset. The hero offset is
//   built member by member (operator* on the def's vector keeps its address in a CPU location). The first digit of three / four is looked
//   up into a named local before the pull-left and clamp (retail's ftol precedes the subtract; the graphic is parked at [ebp+54h]).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#define FABLE_RETAIL_C2DVECTOR
#include "engine/C2DVector.h"
#define FABLE_RETAIL_CPLAYERGUIDEF_DAMAGE
#include "engine/CPlayerGuiDef.h"
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#include "engine/CPlayerGui.h"
#define FABLE_RETAIL_CIENGINE
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#include "engine/CEnginePrimitive2DSprite.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEnginePrimitiveHandle.h"
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#include "engine/NPlayerGui_CDrawBase.h"
#define FABLE_RETAIL_NPLAYERGUI_ANCHORS
#include "engine/NPlayerGui_Graphics.h"
#include "engine/NPlayerGui_CDrawDamage.h"

namespace NPlayerGui {
extern float VALUE_DISPLAY_DURATION;   // 0x013BA860

// Same-TU body of CDrawBase::ScaleAlpha (retail 0x00643D5D, Ego_r.pdb ?ScaleAlpha@CDrawBase@NPlayerGui@@QBEEE@Z): the GUI alpha applied
// through the FableWin.pdb inline CPlayerGui::ScaleAlpha; compiled out of line it gives retail's 29 bytes exactly.
unsigned char CDrawBase::ScaleAlpha(unsigned char alpha) const
{
    return PlayerGui->ScaleAlpha(alpha);
}

void CDrawDamage::Draw(const CEnginePrimitivePrepareInfo& info)
{
    if (DamageValues.empty())
        return;
    Drawing = true;
    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
    std::for_each(PrimitiveHandles.begin(), PrimitiveHandles.end(), std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    PrimitiveHandles.resize(DamageValues.size() * 4);
    long index = 0;
    for (std::list<SDamageValue>::iterator it = DamageValues.begin(); it != DamageValues.end(); ++it) {
        C2DVector pos = GFAnchorToBottomRight(def.DigitPos);
        if (it->Hero)
            pos = GFAnchorToTopLeft(def.DigitHeroPos);
        long duration = it->Duration;
        float time = (float)duration;
        unsigned char alpha = (unsigned char)(255.0f - time / VALUE_DISPLAY_DURATION * 128.0f);
        if (duration < 0) {
            if (duration * 20 < -128)
                alpha = 0;
            else
                alpha = (unsigned char)(it->Duration * 20 + 128);
        }
        C2DVector offset(0.0f, time * 2.0f);
        if (it->Hero)
            offset = C2DVector(def.DigitHeroOffset.X * time, def.DigitHeroOffset.Y * time);
        C2DVector draw_pos = pos + offset;
        if (it->Change >= 0.0f) {
            if (it->Change < 10.0f) {
                CEnginePrimitive2DSprite sprite(0, GFToScreen(draw_pos), ENGINE_2D_LAYER_0, def.DigitGraphics[(long)it->Change], 0,
                                                ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(alpha), 0xff, 0, 0, 2,
                                                C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                info.Engine->SetPersistentPrimitive(PrimitiveHandles[index], sprite, 0xc0);
            } else if (it->Change < 100.0f) {
                draw_pos.X -= 11.0f;
                CEnginePrimitive2DSprite sprite(0, GFToScreen(draw_pos), ENGINE_2D_LAYER_0, def.DigitGraphics[(long)it->Change % 100 / 10], 0,
                                                ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(alpha), 0xff, 0, 0, 2,
                                                C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                info.Engine->SetPersistentPrimitive(PrimitiveHandles[index], sprite, 0xc0);
                draw_pos.X += 22.0f;
                CEnginePrimitive2DSprite sprite2(0, GFToScreen(draw_pos), ENGINE_2D_LAYER_0, def.DigitGraphics[(long)it->Change % 100 % 10], 0,
                                                 ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(alpha), 0xff, 0, 0, 2,
                                                 C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                info.Engine->SetPersistentPrimitive(PrimitiveHandles[index + 1], sprite2, 0xc0);
            } else if (it->Change < 1000.0f) {
                long graphic = def.DigitGraphics[(long)it->Change % 1000 / 100];
                draw_pos.X -= 22.0f;
                if (draw_pos.X < 0.0f)
                    draw_pos.X = 0.0f;
                CEnginePrimitive2DSprite sprite(0, GFToScreen(draw_pos), ENGINE_2D_LAYER_0, graphic, 0,
                                                ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(alpha), 0xff, 0, 0, 2,
                                                C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                info.Engine->SetPersistentPrimitive(PrimitiveHandles[index], sprite, 0xc0);
                draw_pos.X += 22.0f;
                CEnginePrimitive2DSprite sprite2(0, GFToScreen(draw_pos), ENGINE_2D_LAYER_0, def.DigitGraphics[(long)it->Change % 100 / 10], 0,
                                                 ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(alpha), 0xff, 0, 0, 2,
                                                 C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                info.Engine->SetPersistentPrimitive(PrimitiveHandles[index + 1], sprite2, 0xc0);
                draw_pos.X += 22.0f;
                CEnginePrimitive2DSprite sprite3(0, GFToScreen(draw_pos), ENGINE_2D_LAYER_0, def.DigitGraphics[(long)it->Change % 100 % 10], 0,
                                                 ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(alpha), 0xff, 0, 0, 2,
                                                 C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                info.Engine->SetPersistentPrimitive(PrimitiveHandles[index + 2], sprite3, 0xc0);
            } else if (it->Change < 10000.0f) {
                long graphic = def.DigitGraphics[(long)it->Change % 10000 / 1000];
                draw_pos.X -= 33.0f;
                if (draw_pos.X < 0.0f)
                    draw_pos.X = 0.0f;
                CEnginePrimitive2DSprite sprite(0, GFToScreen(draw_pos), ENGINE_2D_LAYER_0, graphic, 0,
                                                ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(alpha), 0xff, 0, 0, 2,
                                                C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                info.Engine->SetPersistentPrimitive(PrimitiveHandles[index], sprite, 0xc0);
                draw_pos.X += 22.0f;
                CEnginePrimitive2DSprite sprite2(0, GFToScreen(draw_pos), ENGINE_2D_LAYER_0, def.DigitGraphics[(long)it->Change % 1000 / 100], 0,
                                                 ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(alpha), 0xff, 0, 0, 2,
                                                 C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                info.Engine->SetPersistentPrimitive(PrimitiveHandles[index + 1], sprite2, 0xc0);
                draw_pos.X += 22.0f;
                CEnginePrimitive2DSprite sprite3(0, GFToScreen(draw_pos), ENGINE_2D_LAYER_0, def.DigitGraphics[(long)it->Change % 100 / 10], 0,
                                                 ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(alpha), 0xff, 0, 0, 2,
                                                 C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                info.Engine->SetPersistentPrimitive(PrimitiveHandles[index + 2], sprite3, 0xc0);
                draw_pos.X += 22.0f;
                CEnginePrimitive2DSprite sprite4(0, GFToScreen(draw_pos), ENGINE_2D_LAYER_0, def.DigitGraphics[(long)it->Change % 100 % 10], 0,
                                                 ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(alpha), 0xff, 0, 0, 2,
                                                 C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                info.Engine->SetPersistentPrimitive(PrimitiveHandles[index + 3], sprite4, 0xc0);
            }
        }
        index += 3;
    }
}
}